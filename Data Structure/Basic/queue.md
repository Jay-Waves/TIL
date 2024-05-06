## 循环队列

循环队列, 比较笨

```c
#define ElementType int
#define Q_SIZE 100
#ifndef _QUEUE_H_
#define _QUEUE_H_
// 大修
    struct Q_NODE{
        ElementType value;
    }queue[Q_SIZE];
    int rear = Q_SIZE-1;//同时处理循环情况和为空情况
    int front = Q_SIZE;
    int count = 0;//记队元素总数

    struct Q_NODE QueuePop(){
        if (count-1 <0)
        {
            printf("error! queue already empty");
            exit(1);
        }
        else
            count--;
        front %= Q_SIZE;//给上一次擦屁股
        return queue[front++];
    }
    void QueuePush(struct Q_NODE tmp){
        if (count+1 > Q_SIZE)
        {
            printf("error! overflow the queue");
            return ;
        }
        count++;
        rear++;
        rear %= Q_SIZE;
        queue[rear].value = tmp.value;
        return;
    }
    void InitQueue( ) {
        rear = Q_SIZE-1;
        front = Q_SIZE;
        count = 0;
    }//每次要重复利用队列时必须重新初始化（该实现方法弊端）

#endif  //_QUEUE_H_
```
- 注意rear是先自增再入值，front是先输出再自增，这导致rear和front的赋值策略是不同的，但是宗旨是入队出队完成后rear和front都指向实际的（第一个和最后一个）数据

