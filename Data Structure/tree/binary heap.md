堆有两个应用:
1. 优先队列 (最小堆)
2. [堆排序](../../../Algorithm/排序/堆排序.md)

https://en.wikipedia.org/wiki/Binary_heap

## 优先队列

堆通常指**二叉堆**, 一种类平衡二叉树的结构. 对于最小堆, 树中任一节点的键值 $k$ 都要大于等于其父节点的 $k$. 

**优先队列**的常见实现方式为**最小堆**. 在优先队列中, 元素键值 $k$ 标识其优先级, 最小键值为最高优先级, 位于堆顶. 访问最小值仅需 $O(1)$ 时间. 假设存在优先队列 $Q$, 向其中插入 $n$ 个元素, 然后再依次取出并删除, 实际上就完成了对这 $n$ 个元素的排序. 由于排序操作的下界为 $O(n\log n)$, 所以堆操作 (插入, 取出, 删除) 的时间复杂度下界大概为 $O(\log n)$.


```c
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/types.h>

#define HEAP_INITIAL_CAPACITY 16

struct min_heap {
    int *data;
    size_t size;
    size_t capacity;
};

/* 初始化堆 */
struct min_heap *heap_create(void)
{
    struct min_heap *heap = kmalloc(sizeof(*heap), GFP_KERNEL);
    if (!heap)
        return NULL;

    heap->data = kmalloc(HEAP_INITIAL_CAPACITY * sizeof(int), GFP_KERNEL);
    if (!heap->data) {
        kfree(heap);
        return NULL;
    }

    heap->size = 0;
    heap->capacity = HEAP_INITIAL_CAPACITY;

    return heap;
}

/* 释放堆 */
void heap_destroy(struct min_heap *heap)
{
    if (heap) {
        kfree(heap->data);
        kfree(heap);
    }
}

/* 堆的上滤操作 */
static void heap_sift_up(struct min_heap *heap, size_t index)
{
    size_t parent = (index - 1) / 2;
    int temp = heap->data[index];

    while (index > 0 && heap->data[parent] > temp) {
        heap->data[index] = heap->data[parent];
        index = parent;
        parent = (index - 1) / 2;
    }
    heap->data[index] = temp;
}

/* 堆的下滤操作 */
static void heap_sift_down(struct min_heap *heap, size_t index)
{
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    size_t smallest = index;
    int temp = heap->data[index];

    if (left < heap->size && heap->data[left] < heap->data[smallest])
        smallest = left;
    if (right < heap->size && heap->data[right] < heap->data[smallest])
        smallest = right;

    if (smallest != index) {
        heap->data[index] = heap->data[smallest];
        heap->data[smallest] = temp;
        heap_sift_down(heap, smallest);
    }
}

/* 插入元素到堆 */
int heap_insert(struct min_heap *heap, int value)
{
    if (heap->size == heap->capacity) {
        size_t new_capacity = heap->capacity * 2;
        int *new_data = krealloc(heap->data, new_capacity * sizeof(int), GFP_KERNEL);
        if (!new_data)
            return -ENOMEM;
        heap->data = new_data;
        heap->capacity = new_capacity;
    }

    heap->data[heap->size] = value;
    heap_sift_up(heap, heap->size);
    heap->size++;

    return 0;
}

/* 删除并返回堆的最小元素 */
int heap_extract_min(struct min_heap *heap, int *min_value)
{
    if (heap->size == 0)
        return -ENODATA;

    *min_value = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heap_sift_down(heap, 0);

    return 0;
}

/* 获取堆的最小元素 */
int heap_peek_min(struct min_heap *heap, int *min_value)
{
    if (heap->size == 0)
        return -ENODATA;

    *min_value = heap->data[0];
    return 0;
}

```

