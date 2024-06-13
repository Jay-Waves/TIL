## 双端队列

双端队列, Double-ended Queue, 允许从两端插入和删除的队列, 操作两侧数据时复杂度为 $O(1)$.

```c
#include "list.h" // based on kernel/include/linux/list.h
#include "errno.h"

struct deque_node {
  int data;
  struct list_head list;
};

void init_deque(struct list_head *deque) {
  INIT_LIST_HEAD(deque);
}

int push_front(struct list_head *deque, int value) {
  struct deque_node *new_node = malloc(sizeof(struct deque_node));
  if (!new_node) {
    return -ENOMEM;
  }
  new_node->data = value;
  list_add(&new_node->list, deque);
}

int push_back(struct list_head *deque, int value) {
  struct deque_node *new_node = malloc(sizeof(struct deque_node));
  if (!new_node) {
    return -ENOMEM;
  }
  new_node->data = value;
  list_add_tail(&new_node->list, deque);
}

int pop_front(struct list_head *deque) {
  if (list_empty(deque)) {
    fprintf(stderr, "Deque is empty\n");
    return -ENOENT;
  }
  struct deque_node *first = 
	  list_first_entry(deque, struct deque_node, list);
  int value = first->data;
  list_del(&first->list);
  free(first);
  return value;
}

int pop_back(struct list_head *deque) {
  if (list_empty(deque)) {
    fprintf(stderr, "Deque is empty\n");
    return -ENOENT;
  }
  struct deque_node *last = 
	  list_last_entry(deque, struct deque_node, list);
  int value = last->data;
  list_del(&last->list);
  free(last);
  return value;
}

void print_deque(struct list_head *deque) {
  struct deque_node *node;
  list_for_each_entry(node, deque, list) {
      printf("%d ", node->data);
  }
  printf("\n");
}
```

总体实现详见: [deque.c](../../src/deque.c)