/* kernel list */
#include <stddef.h> // for size_t

#ifndef _LIST_H
#define _LIST_H

typedef struct list_head {
	struct list_head *next;
	struct list_head *prev;
} list_head;

/*
 * visiting
 */

// access the container of list_head
#define container_of(ptr, type, member) (           \
	(type *)( (char *)ptr - offsetof(type, member) )  )

// add type-check: if ptr is actually a ptr of member.
// also known as `list_entry()`
#define container_of_safe(ptr, type, member) ({                \
	const __typeof__( ((type *) 0)->member ) *__mptr = (ptr);   \
	(type *)( (char *)__mptr - offsetof(type, member) );   })

/*
 * methods
 */

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

#define list_empty(head) \
    ((head)->next == (head) && (head)->prev == (head))

#define list_last_entry(ptr, type, member) \
    container_of((ptr)->prev, type, member)

/*
 * traversing 
 */

// member is the name of list_head in typeof(*pos)
#define list_next_entry(pos, member)     \
	container_of((pos)->member.next, __typeof__(*(pos)), member)

// `ptr` points at the header of linked list, which is independent from data.
#define list_first_entry(ptr, type, member) \
	container_of((ptr)->next, type, member)

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_entry(pos, head, member)                       \
    for (pos = list_first_entry(head, __typeof__(*pos), member); \
         &pos->member != (head);                                     \
         pos = list_next_entry(pos, member))

#endif // _LIST_H
