信号量, semaphore

常作为锁, 定义在 `sys/sem.h` 中, 函数原型:

```c
// key: kernel semophore identifire
// num_sems: number of needed semaphores
// sem_flags: IPC_CREAT | IPC_EXCL
// return sem_id, temp identifier of this semephore (not key); -1 if failed
int semget(key_t key, int num_sems, int sem_flags);  


struct sembuf{  
    short sem_num; // 0, unless using group of semophore
    short sem_op;  // P=-1 (wait), or V=+1(release) 
    short sem_flg; // SEM_UNDO
};

int semop(int sem_id, struct sembuf *sem_opa, size_t num_sem_ops);  
```