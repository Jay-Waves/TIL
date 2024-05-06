```c
#define ElementType int
#define STK_SIZE 100
#ifndef _STACK_H_
#define _STACK_H_

    struct S_NODE{
        ElementType value;
    }stack[STK_SIZE];
    int sTop = -1;
    struct S_NODE StackPop(){
        if (sTop == -1)
        {
            printf("error! out of space with index -1");
        }
        return stack[sTop--];
    }
    void StackPush(struct S_NODE tmp){
        if (sTop == STK_SIZE)
        {
            printf("error! out of space with overflow");
        }
        stack[++sTop].value = tmp.value;
        return;
    }

#endif  //_STACK_H_
```

```c

```