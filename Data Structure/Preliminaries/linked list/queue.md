## 队列

队列, 指先入先出 (FIFO) 数据结构, 先加入队列的元素先出队列得到处理.

```
out <- node <- node <- node <- node <- in
```

linux 的队列实现称为 kfifo, 详见 [kernel/kfifo](linux%20kernel/kfifo.md)

### 循环队列

队列尾接到队列头, 逻辑上隐藏了列表尾部, 避免了"假溢出"问题.

```c
struct circular_queue {
  int front;
  int rear;
  int size;
  int capacity;
  int buffer[];
};

struct circular_queue *init(int capacity)
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

bool is_empty(struct circular_queue *q)
{
  return q->size == 0;
}

bool is_full(struct circular_queue *q)
{
  return q->size == q->capacity;
}

bool in(struct circular_queue *q, int value)
{
  if (is_full(q)) {
    return false;
  }
  q->rear = (q->rear + 1) % q->capacity;
  q->buffer[q->rear] = value;
  q->size++;
  return true;
}

bool out(struct circular_queue *q, int *value)
{
  if (is_empty(q)) {
    return false; 
  }
  *value = q->buffer[q->front];
  q->front = (q->front + 1) % q->capacity;
  q->size--;
  return true;
}

bool peek(struct circular_queue *q, int *value)
{
  if (is_empty(q)) {
    return false;
  }
  *value = q->buffer[q->front];
  return true;
}
```

代码详见 [queue.c](../../../src/queue.c)