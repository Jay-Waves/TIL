`kfifo` is the linux kernel's implementation of queue, first data in is the first data out. kfifo is a **circular queue** defined in `include/linux/kfifo.h`.

```c
// size of kfifo is often set 2^n, in order to use bitwise instead of modulus 
// to update index `in` and `out`. 
// if `in=out`, buffer is empty; if `(in - out)>mask`, buffer is fullfilled.
struct kfifo {
    void *buffer;    
    unsigned int esize;  /* element size */
    unsigned int mask;   /* size of kfifo, 2^n */
    unsigned int in;     /* index of queue in */
    unsigned int out;    /* index of queue out */
};

// init kfifo, return 0 on success.

int kfifo_alloc(struct kfifo *fifo, unsigned int size, gfp_t gfp_mask);
void kfifo_free(struct kfifo *fifo);

unsigned int kfifo_in(struct kfifo *fifo, const void *from, unsigned int len);
unsigned int kfifo_out(struct kfifo *fifo, const void *to, unsigned int len);
unsigned int kfifo_out_peek(struct kfifo *fifo, const void *to, 
													unsigned int len, unsigned offet);

/**
 * kfifo_len - returns the number of used elements in the fifo
 * @fifo: address of the fifo to be used
 */
#define kfifo_len(fifo) \
({ \
	typeof((fifo) + 1) __tmpl = (fifo); \
	__tmpl->kfifo.in - __tmpl->kfifo.out; \
})

/**
 * kfifo_is_empty - returns true if the fifo is empty
 * @fifo: address of the fifo to be used
 */
#define	kfifo_is_empty(fifo) \
({ \
	typeof((fifo) + 1) __tmpq = (fifo); \
	__tmpq->kfifo.in == __tmpq->kfifo.out; \
})

/**
 * kfifo_is_full - returns true if the fifo is full
 * @fifo: address of the fifo to be used
 */
#define	kfifo_is_full(fifo) \
({ \
	typeof((fifo) + 1) __tmpq = (fifo); \
	kfifo_len(__tmpq) > __tmpq->kfifo.mask; \
})
```

这里的宏用法:
- `fifo+1` 利用了 c 指针特性, 其地址实际增加的值为 `sizeof(kfifo)`. 实际隐式检查了 fifo 的类型.
- `typeof()` 声明 `__tmpq` 为 `fifo` 同类型.
- `__tmpq = (fifo)` 用临时变量存储 fifo, 避免传入的是右值 (无法复用), 如传入 `kfifo_get(&my_fifo)`

```c
/*
 * internal helper to calculate the unused elements in kfifo.
 */
static inline unsigned int kfifo_unused(struct __kfifo *fifo)
{
	return (fifo->mask + 1) - (fifo->in - fifo->out);
}

// similar as kfifo_init, which accepts outside buffer array.
int kfifo_alloc(struct kfifo *fifo, unsigned int size, 
								size_t esize, gfp_t gfp_mask)
{
	/*
	 * round up to the next power of 2, since our 'let the indices
	 * wrap' technique works only in this case.
	 */
	size = rounddown_pow_of_two(size);
	fifo->in = 0;
	fifo->out = 0;
	fifo->esize = esize;

	if (size < 2) {
		fifo->data = NULL;
		fifo->mask = 0;
		return -1; // -EINVAL
	}

	fifo->data = kmalloc(size *esize, gfp_mask);
	if (fifo->data == NULL) {
		fifo->mask = 0;
		return -1; // -ENOMEM
	}
	fifo->mask = size - 1;
	return 0;
}

void kfifo_free(struct kfifo *fifo)
{
	kfree(fifo->data);
	fifo->data = NULL;
	fifo->in = 0;
	fifo->out = 0;
	fifo->esize = 0;
	fifo->map = 0;
}

static void kfifo_copy_in(struct __kfifo *fifo, const void *src,
		unsigned int len, unsigned int off)
{
	unsigned int size = fifo->mask + 1;
	unsigned int esize = fifo->esize;
	unsigned int l;

	off &= fifo->mask;
	if (esize != 1) {
		off *= esize;
		size *= esize;
		len *= esize;
	}
	l = min(len, size - off);

	memcpy(fifo->data + off, src, l);
	memcpy(fifo->data, src + l, len - l);
	/*
	 * make sure that the data in the fifo is up to date before
	 * incrementing the fifo->in index counter
	 */
	smp_wmb();
}

unsigned int kfifo_in(struct kfifo *fifo, const void *buf, unsgned int len)
{
	unsigned int l;
	l = kfifo_unused(fifo);
	if (len > l)
		len = l;
	kfifo_copy_in(fifo, buf, len, fifo->in);
	return len;
}

static void kfifo_copy_out(struct __kfifo *fifo, void *dst,
		unsigned int len, unsigned int off)
{
	unsigned int size = fifo->mask + 1;
	unsigned int esize = fifo->esize;
	unsigned int l;

	off &= fifo->mask;
	if (esize != 1) {
		off *= esize;
		size *= esize;
		len *= esize;
	}
	l = min(len, size - off);

	memcpy(dst, fifo->data + off, l);
	memcpy(dst + l, fifo->data, len - l);
	/*
	 * make sure that the data is copied before
	 * incrementing the fifo->out index counter
	 */
	smp_wmb();
}

unsigned int __kfifo_out_peek(struct __kfifo *fifo,
		void *buf, unsigned int len)
{
	unsigned int l;

	l = fifo->in - fifo->out;
	if (len > l)
		len = l;

	kfifo_copy_out(fifo, buf, len, fifo->out);
	return len;
}

unsigned int __kfifo_out_peek(struct __kfifo *fifo,
		void *buf, unsigned int len)
{
	unsigned int l;

	l = fifo->in - fifo->out;
	if (len > l)
		len = l;

	kfifo_copy_out(fifo, buf, len, fifo->out);
	return len;
}
EXPORT_SYMBOL(__kfifo_out_peek);

unsigned int __kfifo_out(struct __kfifo *fifo,
		void *buf, unsigned int len)
{
	len = __kfifo_out_peek(fifo, buf, len);
	fifo->out += len;
	return len;
}
EXPORT_SYMBOL(__kfifo_out);


```

### 示例

```c
#include <linux/kfifo.h>
#include <linux/kernel.h>
```