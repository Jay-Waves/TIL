```
list
 v
node <-> node <-> ... <-> node 
```

## doubly_linked_list.h

这里提供一个*无头*的双向链表实现. 无头指没有独立节点作为链表的逻辑头部, 此时插入删除操作都会更复杂. 鉴于链表十分简洁灵活, 示例中将不支持泛型, 仅用 `int` 代表每个节点的数据类型, 使用非入侵式设计 (`int` 可以替换为指向任何数据的指针).

```c
#ifndef _DLL_H
#define _DLL_H

 // typdef is not recommended in kernel
// typedef int data_t

typdef struct dll_node {
	int val; /* use `int` for simplicity */
	struct dll_node *next, *prev;
	/* int ref_cnt; */
} dll_node;

#define dll_for_each(pos, head) \
	for(pos=head; pos!=NULL; pos=pos->next)

/*
 * create the first node, representing a new doubly linked list.
 * can also used in create new node.
 * @param val value of the first node
 * @return list pointer at new doubley linked list.
 */
dll_node *dll_new(int val);

/*
 * find poisiton of node with value `val` in the list. O(n/2)
 * @return pos pointer at the node with `val`, NULL if not found.
 */
dll_node *dll_find(int val, dll_node *list);

/*
 * append a new node at the tail of lsit. O(n)
 * if list is NULL, 
 */
void dll_append(int val, dll_node *list);

/*
 * `void dll_prepend(int val, struct dll_node **list);`
 * prepend a new node at the beginning of list. O(1)
 * because dll neither has header nor is a circular, external pointer
 * should be updated whenever prepending new node on the list.
 */

/*
 * Find and remove a val inside list, as well as free node's memory. 
 * Rely on dll_find()'s O(n/2). 
 * If the deleted node is the head, external ptr will be updated to head->next.
 * That's say, If the list has only one node, which is exactly the deleted, 
 * external ptr will be NULL.
 */
void dll_remove(int val, dll_node **list);

#endif /* _DLL_H */
```

## doubly_linked_list.c

```c
#include "doubly_linked_list.h"
#include <assert.h>
#include <stdlib.h>

dll_node *dll_new(int val)
{
	dll_node *list = (dll_node *)malloc(sizeof(dll_node));
	assert(list != NULL);
	list->val = val;
	list->next = NULL;
	list->prev = NULL;
	return list;
}

dll_node *dll_find(int val, dll_node *list)
{
	assert(list != NULL);
	if (list->val == val) return list;
	return dll_find(val, list->next); 
	//! careful about stack overflow with big list
}

void dll_append(int val, dll_node *list)
{
	assert(list != NULL);
	dll_node *tail;
	for (tail = list; tail->next != NULL; tail = tail->next); 
	// now `pos` at original tail
	tail->next = dll_new(val);
	tail->next->prev = tail;
}

// with node->prev, remove() will be convenient.
void dll_remove(int val, dll_node **list)
{
	assert(*list != NULL);
	dll_node *pos = dll_find(val, *list);
	if (pos == NULL) return; /* val not found */
	if (pos->prev == NULL){
		*list = pos->next; 
	} else { // not the header
		pos->prev->next = pos->next;
	}
	if (pos->next != NULL) { // not the tail
		pos->next->prev = pos->prev; 
	}
	free(pos);
}
```

`ddl_remove` 还有几种解决方式:
- 添加独立头节点, 规避链表首节点就是要删除的节点的情况. 缺点是尾元素仍是边界条件.
- 惰性删除, 即不实际删除, 仅用 `flag` 或特殊值标记.
- 值替换. 当需要删除首个节点时, 为避免外部对链表的引用失效, 将第二个节点的值拷贝到首节点, 同时删除第二个节点. 缺点是, 无法处理仅剩一个节点的情况.
- [circular linked list](circular%20linked%20list.md). 缺点是每次访问都需要判断节点是否为空.
