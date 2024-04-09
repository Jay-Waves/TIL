```c
struct msg_queue {
    struct kern_ipc_perm q_perm;
    time_t q_stime;        /* last msgsnd time */
    time_t q_rtime;        /* last msgrcv time */
    time_t q_ctime;        /* last change time */
    unsigned long q_cbytes;    /* current number of bytes on queue */
    unsigned long q_qnum;      /* number of messages in queue */
    unsigned long q_qbytes;    /* max number of bytes on queue */
    pid_t q_lspid;          /* pid of last msgsnd */
    pid_t q_lrpid;          /* last receive pid */
    struct list_head q_messages;
    struct list_head q_receivers;
    struct list_head q_senders;
};
```