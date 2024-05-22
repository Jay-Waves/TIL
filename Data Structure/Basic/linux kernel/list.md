`./include/linux/list.h` 中定义了内核使用的[双向循环列表](../linked%20list/circular%20linked%20list.md). 内核链表使用入侵式设计, 即将链表头嵌入其他数据结构的末尾来使用, 链表接口不负责相关内存管理 (即, 不假设数据是在栈还是在堆实现的).

```c
typdef struct list_head {
	struct list_head *next;
	struct list_head *prev;
} list_head;

struct data {
	int a;
	int b;
	list_head list;
}
```

入侵式链表, 要通过 `list_head` 获取其所在容器 `data`, 需要手动用偏移计算地址.

```c
/*
 * The trick here is that `(st *)0` is equivalent to `(st *)NULL`, 
 * indicating that the new ptr of `st` points at the zero address.
 * for example: offsetof(struct data, a) = &data.a - &data
 */
#define offsetof(st, m) ( (size_t) &(((st *)0)->m) )

// access the container of list_head
#define container_of_(ptr, type, member) (           \
	(type *)( (char *)ptr - offsetof(type, memeber) )  )

// add type-check: if ptr is actually a ptr of member.
// also known as `list_entry()`
#define container_of(ptr, type, member) ({                \
	const typeof( ((type *) 0)->member ) *__mptr = (ptr);   \
	(type *)( (char *)__mptr - offsetof(type, member) );   })
```

内置表结构管理方法:

```c
#include <linux/list.h>

// initialize a list_head's prev/next pointer at runtime.
#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); \
    (ptr)->prev = (ptr); \
} while (0)

// get a linked_list_head variable named `name` at compile time.
#define LIST_HEAD(name)                        \
    struct list_head name = { &(name), &(name) }

#define list_add(new, head) do {               \
    (new)->next = (head)->next;                \
    (new)->next->prev = (new);                 \
    (new)->prev = (head);                      \
    (head)->next = (new);                      \
} while (0)

// add new at the end of list (immeadiately before head)
#define list_add_tail(new, head) do {        \
    struct list_head *_prev = (head)->prev;  \
    (new)->next = (head);                    \
    (new)->prev = _prev;                     \
    _prev->next = (new);                     \
    (head)->prev = (new);                    \
} while (0)

// remove entry from the list structure, but not free its memory
#define list_del(entry) do {                    \
    struct list_head *_prev = (entry)->prev;    \
    struct list_head *_next = (entry)->next;    \
    _next->prev = _prev;                        \
    _prev->next = _next;                        \
    (entry)->next = (entry)->prev = NULL;       \
} while (0)
```

### 遍历方法

```c
// member is the name of list_head in typeof(*pos)
#define list_next_entry(pos, member)     \
	container_of((pos)->member.next, typeof(*(pos)), member)

// `ptr` points at the header of linked list, which is independent from data.
#define list_first_entry(ptr, type, member) \
	container_of((ptr)->next, type, member)

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_entry(pos, head, member)                       \
    for (pos = list_first_entry(head, typeof(*pos), member); \
         &pos->member != (head);                                     \
         pos = list_next_entry(pos, member))

```

### 用法示例

```c
#include <linux/list.h>
#include <linux/slab.h>

struct data {
	char c;
	struct list_head list;
};

void test( void ) 
{
	LIST_HEAD(head); // at runtime
	struct data *d, *next;
	char *s = "hello, world!";

	for (; *s; s++) {
		d = kmalloc(sizeof(struct data), GFP_KERNEL);
		d->c = *s;
		INIT_LIST_HEAD(&d->list);
		list_add(&d->list, &head);

		list_for_each_entry(d, &head, list) {
			printk("%c\n", d->c);
		}

		list_for_each_entry(d, &head, list) {
			lsit_del(&d->list);
			kfree(d);
		}
	}
}
```

***

调侃一下, 这些宏颇有种把 c 当 python 写的味:

```c
                             int 
main                         (){int
	i = 0                      ;
	for (; i!=10; i++)         {
		if (i>5)                 {
			printf("hello")        ;}}}
```