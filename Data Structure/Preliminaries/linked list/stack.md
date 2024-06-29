栈有两种常见实现方法:

| 实现                                         | 随机访问效率 | 空间利用效率 |
| -------------------------------------------- | ------------ | ------------ |
| 数组实现, 简单高效                           | `O(1)`       | 无法动态变化 |
| 链表实现, 参考 [list](linked%20list/list.md) | `O(n)`       | 动态分配空间             |

数组和链表的区别分析详见 [Array vs Linked List](linked%20list%20or%20array.md)

## 数组实现

```c
struct stack_item {
	int val;
} 

struct stack {
	struct stack_item *items;
	size_t top;
	size_t capacity;
};

#define STACK_EMPTY -1
#define stack_is_empty(s) ((s)->top == STACK_EMPTY)
#define stack_is_full(s) ((s)->top == (s)->capacity - 1)
#define stack_size(s) ((s)->top + 1)
#define stack_capacity(s) ((s)->capacity)

bool stack_init(struct stack *s, size_t capacity)
{
	if (capacity <= 0)
		return false;
	s->items = malloc(capacity * sizeof(struct stack_item));	
	assert(s->items != NULL);
	s->top = STACK_EMPTY;
	s->capacity = capacity;
	return true;
}

bool stack_pop(struct stack *s, struct stack_item *item) 
{
	if (stack_is_empty(s)) {
		return false;
	}
	*item = s->items[s->top];
	s->top--;
	return true;
}

bool stack_peek(struct stack *s, struct stack_item *item)
{
	if (stack_is_empty(s)) {
		return false;
	}
	// increase counter of reference, because our stack still holds 
	// the pointer to peeked stack_item.
	*item = s->items[s->top];
	return true;
}

bool stack_push(struct stack *s, struct stack_item* item) 
{
	if (stack_is_full(s))
		return false;
	s->top++;
	s->items[s->top] = *item;
	return true;
}

void stack_resize(struct stack *s, size_t capacity) {
	if (capacity > s->capacity) {
		size_t size = capacity * sizeof(struct stack_item);
		struct stack_item *items = realloc(s->items, size); 
		assert(items != NULL);
		s->items = items;
	}
	s->capacity = capacity;
}
```

