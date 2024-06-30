|          | array                                                           | list                           |
| -------- | --------------------------------------------------------------- | ------------------------------ |
| 访问     | 随机访问                                                        |                                |
| 查询     | 线性查找, 可用二分查找                                          | 线性查找                       |
| 插入     | 移动 `n/2` 个元素                                               | 重链接                         |
| 删除     | 移动 `n/2` 个元素                                               |                                |
| 内存大小 | 大小不可变, 调整大小时需声明新内存, <br> 然后将数据完全复制过去 | 大小可变                       |
| 内存使用 | malloc头 + 数组头 + 数据                                        | 数据 + N\*malloc头 + N\*链表头 |

Bjarne Stroustrup 建议能用 `std::vector` (类似 array), 就不要用 `linked list`. 
因为**链表较难利用 CPU 缓存技术**, 即由于其节点内存是离散的, 对于 CPU 而言是很难预测的, 因此后续链表节点不会被载入缓存, 导致每次都需要去内存中取数据. 整个过程浪费了200~500个指令周期: TLB未命中, 查页表, 地址转换, 内存访问等操作, 非常慢.

**要充分利用 CPU 缓存和预测, 需要保持数据的紧凑性.** 对于小块数据, 减少指针重定向的次数, 比如每个数据元仅存一个整型就完全没必要用链表, 也不必建一个结构体或类.

## Array

数组指**连续空间, 相同类型**的一组数据, 需要在**编译时**声明大小和类型. 

```
address(a[k]) = base_address + k * sizeof(type)
```

如果数组下标从 `1` 开始, 取址式子如下, 可以看到多了一步减法操作. 

```
address(a[k]) = base_address + (k-1) * sizof(type)
```

## Linked List

线性表, 每个数据节点之间通过指针相连, 内存中数据节点不连续. 链表可在**运行时**动态变化大小, 通过 `malloc()` 等分配离散内存.

```
node -> node -> node -> node
```

## Array Resize

链表的一大优势是动态调整空间, 数组也可以模拟这一点.

算法描述如下: 
- 当占用比例大于 0.7 (某个阈值) 时, 将哈希表尺寸扩大一倍.
- 当占用比例小于 0.1 时, 将哈希表尺寸缩小一倍.

该算法分配内存的次数为 $O(log_{2}N)$, `c++ std::vector` 也是类似办法. 优于链表的内存分配次数 $O(N)$, 但增加了数据复制的时间. 对于 `insert(), delete()` 等例程, 虽然偶尔会有 `resize()` 开销, 但根据均摊分析, 实际均摊时间复杂度仍为 $O(1)$.

```c
int is_prime(const int x);
int next_prime(int x);

const int BASE_SIZE=50
typedef struct {
	int base_size;
	int size;  /* size of memory, first prime bigger than base size */
	int count; /* number of items */
	arr_item** items;
} arr;

// in .c
static arr* new_sized(int base_size) 
{
	arr* a = malloc(sizeof(arr));
	a->base_size = base_size;
	a->size = next_prime(base_size);

	a->count = 0;
	a->items = calloc((size_t)a->size, sizeof(arr_item*));
	return a;
}

arr* new() 
{
	return new_sized(BASE_SIZE);
}

static void resize(arr* a, int new_base_size)
{
	if(new_base_size < BASE_SIZE) /* too small */
		return;
	arr* new_a = new_sized(new_base_size);
	for(int i = 0; i < a->size; i++){
		arr_item* item = a->items[i];
		if(item != NULL){
			// insert item into new_a->items, we donot know the args.
			insert(new_a, ...);
		}
	}

	// swap a and new_a. keep using pointer a, instead of new_a.
	a->base_size = new_a->base_size;
	a->count = new_a->count;
	delete_items(/*items ptr*/ a->items, /*length*/ a->size);
	a->size = new_a->size;
	a->items = new_a->items;
	free(new_a);
}

static void resize_up(arr* a)
{
	const int new_size = a->base_size * 2;
	resize(a, new_size);
}

static void resize_down(arr* a)
{
	const int new_size = a->base_size / 2;
	resize(a, new_size);
}

void insert(arr* a, ...) 
{
	const int load = a->count * 100 / a->size;
	if(load > 70)
		resize_up(a);
	...
}

void delete(arr* a, ...) 
{
	const int load = a->count * 100 / a->size;
	if(load < 10)
		resize_down(a);
	...
}
```


