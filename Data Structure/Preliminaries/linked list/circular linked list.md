循环链表指将链表首节点视为其尾部节点的下一个节点, 形成环状结构. 巧妙地避免了无头列表首尾元素的边界情况. 很适合无序数据结构.

```
 +-----------------------+
 |                       |
 v                       |
node -> node -> ...  -> node
```

## circular_doubly_linked_list.h

**有头双端循环链表**是内核中使用的链表形式, 没有头尾之分, 任一节点都可以作为遍历的起始或结尾. 
- *[双端](doubly%20linked%20list.md)*指节点既有指向前一个节点的指针, 也有指向下一个节点的指针; 
- *有头*指有独立节点作为链表的逻辑头部, 所有节点逻辑上保持一致.

```
                  data           data              data
                   v              v                 v
-> list_head <-> head(first) <-> head <-> ... <-> head(last) <-
```

这里的示例将使用入侵式设计, 即数据结构的管理信息 (如前驱后驱指针) 被嵌入到数据结构本身之中, 常用于无 GC 的语言. 同时, 将**内存管理**和链表结构管理分离, 不假设数据是在栈分配还是在堆分配, 交由用户自行管理内存.

```c
#ifndef _CDLL_H
#define _CDLL_H

typedef struct cdll_head {
	struct cdll_head *next, *prev;
} cdll_head;

struct item {
	int data;
	cdll_head list;
}

/*
 * initlize list header.
 */
void cdll_init_head(cdll_head *list);

/*
 * insert a node between previous node and next node.
 */
static void cdll_add(cdll_head *new, cdll_head *prev, cdll_head *next);

/*
 * insert a node right after header
 */
void cdll_prepend(cdll_head *new, cdll_head *head);
/*
 * insert a node before header, at the tail.
 */
void cdll_append(cdll_head *new, cdll_head *head);
/*
 * separate node from list structure, but not free its memory.
 */
void cdll_delete(cdll_head *entry);

#define cdll_empry(head) (head->next == head)
#define cdll_singular(head) (head->next == head->prev)

#define offsetof(st, m) ( (size_t) &(((st *)0)->m) )
#define cdll_for_each(pos, head) \
	for(pos=head; pos!=NULL; pos=pos->next)

#endif /* _CDLL_H */
```

## circular_doubly_linked_list.c

```c

void cdll_init_head(cdll_head *list)
{
	list->next = list;
	list->prev = list;
}

static void cdll_add(cdll_head *new, cdll_head *prev, cdll_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

void cdll_prepend(cdll_head *new, cdll_head *head) 
{
    cdll_add(new, head, head->next);
}

void cdll_append(cdll_head *new, cdll_head *head) 
{
    cdll_add(new, head->prev, head);
}

void cdll_delete(cdll_head *entry) 
{
    cdll_head *prev = entry->prev;
    cdll_head *next = entry->next;
    next->prev = prev;
    prev->next = next;
}

static void cdll_print(cdll_head *head) {
    list_head *i;
    printf("List: ");
    for (i = head->next; i != head; i = i->next) {
        struct item *item = (struct item *)(
	        (char *)i - offsetof(struct item, list)
	      );
        printf("%d ", item->data);
    }
    printf("\n");
}
```

linux 内核的链表就是循环双向链表, 详见 [kernel/list](../linux%20kernel/list.md).

