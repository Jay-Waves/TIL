#include <stdio.h>
#include <stdlib.h>
#include "list.h" 
#include "errno.h"

struct deque_node {
  int data;
  struct list_head list;
};

static void init_deque(struct list_head *deque) {
  INIT_LIST_HEAD(deque);
}

static int push_front(struct list_head *deque, int value) {
  struct deque_node *new_node = malloc(sizeof(struct deque_node));
  if (!new_node) {
    return -ENOMEM;
  }
  new_node->data = value;
  list_add(&new_node->list, deque);
  return 0;
}

static int push_back(struct list_head *deque, int value) {
  struct deque_node *new_node = malloc(sizeof(struct deque_node));
  if (!new_node) {
    return -ENOMEM;
  }
  new_node->data = value;
  list_add_tail(&new_node->list, deque);
  return 0;
}

static int pop_front(struct list_head *deque) {
  if (list_empty(deque)) {
    fprintf(stderr, "Deque is empty\n");
    return -ENOENT;
  }
  struct deque_node *first = list_first_entry(deque, struct deque_node, list);
  int value = first->data;
  list_del(&first->list);
  free(first);
  return value;
}

static int pop_back(struct list_head *deque) {
  if (list_empty(deque)) {
    fprintf(stderr, "Deque is empty\n");
    return -ENOENT;
  }
  struct deque_node *last = list_last_entry(deque, struct deque_node, list);
  int value = last->data;
  list_del(&last->list);
  free(last);
  return value;
}

static void print_deque(struct list_head *deque) {
  struct deque_node *node;
  list_for_each_entry(node, deque, list) {
      printf("%d ", node->data);
  }
  printf("\n");
}

int main() {
  LIST_HEAD(my_deque);
  init_deque(&my_deque);

  push_front(&my_deque, 10);
  push_back(&my_deque, 20);
  push_front(&my_deque, 5);
  push_back(&my_deque, 30);

  printf("Deque after insertions: ");
  print_deque(&my_deque);

  printf("Popped from front: %d\n", pop_front(&my_deque));
  printf("Popped from back: %d\n", pop_back(&my_deque));

  printf("Deque after deletions: ");
  print_deque(&my_deque);

  return 0;
}
