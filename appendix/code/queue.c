#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct circular_queue {
  int front;
  int rear;
  int size;
  int capacity;
  int buffer[];
};

static struct circular_queue *init(int capacity)
{
  struct circular_queue *q = (struct circular_queue*) malloc (
      sizeof(struct circular_queue) + capacity * sizeof(int)
  );
  if (!q) exit(EXIT_FAILURE);
  
  q->front = 0;
  q->rear = -1;
  q->size = 0;
  q->capacity = capacity;
  return q;
}

static bool is_empty(struct circular_queue *q)
{
  return q->size == 0;
}

static bool is_full(struct circular_queue *q)
{
  return q->size == q->capacity;
}

static bool in(struct circular_queue *q, int value)
{
  if (is_full(q)) {
    return false;
  }
  q->rear = (q->rear + 1) % q->capacity;
  q->buffer[q->rear] = value;
  q->size++;
  return true;
}

static bool out(struct circular_queue *q, int *value)
{
  if (is_empty(q)) {
    return false; 
  }
  *value = q->buffer[q->front];
  q->front = (q->front + 1) % q->capacity;
  q->size--;
  return true;
}

static bool peek(struct circular_queue *q, int *value)
{
  if (is_empty(q)) {
    return false;
  }
  *value = q->buffer[q->front];
  return true;
}

static void print(struct circular_queue *q)
{
    if (is_empty(q)) {
        printf("Queue is empty\n");
        return;
    }

    printf("Queue elements: ");
    for (int i = 0; i < q->size; i++) {
        int index = (q->front + i) % q->capacity;
        printf("%d ", q->buffer[index]);
    }
    printf("\n");
}

int main(void)
{
    struct circular_queue *q;
    int val;

    q = init(10);

    in(q, 1);
    in(q, 2);
    in(q, 3);
    in(q, 4);
    in(q, 5);
    print(q);

    peek(q, &val);
    printf("PeekOut: %d\n", val);
    out(q, &val);
    printf("Dequeued: %d\n", val);
    print(q);

    while (out(q, &val)) {
        printf("Dequeued: %d\n", val);
    }
    print(q);

    return 0;
}
