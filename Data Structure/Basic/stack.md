栈有两种常见实现方法:

| 实现                                         | 随机访问效率 | 空间利用效率 |
| -------------------------------------------- | ------------ | ------------ |
| 数组实现, 简单高效                           | `O(1)`       | 无法动态变化 |
| 链表实现, 参考 [list](linked%20list/list.md) | `O(n)`       | 动态分配空间             |

## 数组实现

```c
#ifndef _STACK_H_
#define _STACK_H_

struct stack_item {
	int val;
	int ref_count;
} 

// never visit stack directly.
struct stack {
	struct stack_item *items;
	size_t top;
	size_t size;
};

#define STACK_EMPTY -1
#define stack_empty(s) ((s)->top == STACK_EMPTY)
#define stack_size(s) ((s)->size)
void stack_init(struct stack *s, size_t stack_size);
void stack_resize(struct stack *s, size_t new_size);
struct stack_item *stack_pop(struct stack *s);
struct stack_item *stack_top(struct stack *s);
bool stack_push(struct stack *s, int val);

#endif  /* _STACK_H_ */


void stack_init(struct stack *s, size_t stack_size)
{
	if (stack_size <= 0)
		return NULL;
	s->items = malloc(stack_size * sizeof(struct stack_item*));	
	assert(s->items != NULL);
	s->top = STACK_EMPTY;
	s->size = stack_size;
}

struct stack_item *stack_pop(struct stack *s) 
{
	if (s->top == STACK_EMPTY)
		return NULL;
	return s->items[s->top--];
}

struct stack_item *stack_top(struct stack *s)
{
	if (s->top == STACK_EMPTY)
		return NULL;
	size_t top = s->top;
	// increase counter of reference, because our stack still holds 
	// the pointer to peeked stack_item.
	s->items[top]->ref_count += 1;
	return s->items[top];
}

bool stack_push(struct stack *s, int val) 
{
	if (s->top == s->size)
		return False;
	struct stack_item *entry = malloc(sizeof(struct stack_item));
	if (entry != NULL)
		return False;
	entry->val = val;
	entry->ref_count = 1;
	s->items[++s->top] = entry;
	return True;
}

void stack_resize(struct stack *s, size_t new_size) {
	if (new_size > s->size) {
		size_t items_size = new_size * sizeof(struct stack_items*)
		void *items = realloc(s->items, items_size); 
		assert(items != NULL)
		s->items = items;
	}
	s->size = new_size;
}
```

