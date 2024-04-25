MPI, Message Passing Interface, 是一种基于消息传递的并发工具库 (的标准).

```c
#include "mpi.h"
#include <stdio.h>

int main( argc, argv)
int argc;
char *argv [];
{
	int myid, numprocs;
	int namelen;
	char proc_name[MIP_MAX_PROCESSOR_NAME];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Get_processor_name(proc_name, &namelen);
	...
	MPI_Finalize();
}
```

编译:
```bash
sudo apt-get install libopenmpi-dev

mpicxx -o hello hello.cc
mpirun -np 4 ./hello
```

## 数据类型

| MPI datatype         | C datatype          |
|----------------------|---------------------|
| MPI_CHAR             | signed char         |
| MPI_SHORT            | signed short int    |
| MPI_INT              | signed int          |
| MPI_LONG             | signed long int     |
| MPI_UNSIGNED_CHAR    | unsigned char       |
| MPI_UNSIGNED_SHORT   | unsigned short int  |
| MPI_UNSIGNED_INT     | unsigned int        |
| MPI_UNSIGNED_LONG    | unsigned long int   |
| MPI_FLOAT            | float               |
| MPI_DOUBLE           | double              |
| MPI_LONG_DOUBLE      | long double         |
| MPI_BYTE             |                     |
| MPI_PACKED           |                     |


## 基础函数原型

```c
// call only once at the beginning of MPI env
int MPI_Init(int *argc, char ***argv)

// finalize current MPI environment
int MPI_Finalize(void)

// get the identifier, `rank`, of current process 
// in current communicator `comm`.
int MPI_Comm_rank(MPI_Comm comm, int *rank)

// get the number of processes in communicator `comm`
int MPI_Comm_size(MPI_Comm comm, int *size)

// Send `datatype`*`count` to process of rank `dest`
// with unique `tag` under the same communicator `comm`.
// Real data stored in generic buffer `void* buf`.
// value: 
//     dest: 0 - NPROCS-1, or MPI_PROC_NULL
//     tag: 0 - MPI_TAG_UB, or MPI_ANY_TAG
//     source: 0 - NPROCS-1, or MPI_ANY_SOURCE
//     comm: MPI_COMM_WOLRD, pre-defined global communicator.
int MPI_Send(void* buf, int count, MPI_Datatype datatype, int dest,
						int tag, MPI_Comm comm)

typedef struct MPI_Status {
	int MPI_SOURCE; // rank of source process
	int MPI_TAG;    // tag of msg
	int MPI_ERROR;  // error code
} MPI_Status;

int MPI_Recv(void* buf, int count, MPI_Datatype datatype, int source, 
						int tag, MPI_Comm comm, MPI_Status* status)

// read and send at the same time
int MPI_Sendrecv(void* snedbuf, int sencount, MPI_Datatype snedtype, 
								 int dest, int sendtag,
								 void* recvbuf, int recvcount, MPI_Datatype recvtype,
								 int source, int recvtag,
								 MPI_Comm comm, MPI_Status* status)
```

### 时钟函数原型

```c
// return wall clock time.
double MPI_Wtime(void)

// return time precision
double MPI_Wtick()
```

### MPI管理函数原型

```c
// check if MPI is already intialized with `MPI_Init()`
int MPI_Initialized(int* flag)

// abort all processes in MPI communicator
int MPI_Abort(MPI_Comm comm, int errorcode)
```

## 通信模式

缓存模式, 将发送消息拷贝至某用户提供的缓冲区后立即返回, 消息发送由MPI后台执行.

```c
// same args as MPI_Send
int MPI_Bsend(...)

// request buffer
int MPI_Buffer_attach(void *buf, int size)

int MPI_Buffer_detach(void *buf, int size)
```

同步模式, MPI发送消息时必须确认接收方已经开始接收消息后才返回.

```c
int MPI_Ssend(...)
```

就绪模式, 发送时须先确认接收方已经处于就绪态, 否则该调用会产生错误.

```c
int MPI_Rsend(...)
```

默认标准模式下, MPI自动决定是否使用缓存. 通常系统会预留一些缓冲区, 缓冲区足够或未满时会拷贝后理解返回; 否则会和接收方联系.

### 阻塞与非阻塞

```c
// non-blocking
MPI_Isend(..., MPI_Request *)

MPI_Irecv(..., MPI_Comm, MPI_Request*)

// wait and test non-blocking communication's completion
int MPI_Wait(MPI_Request *, MPI_Status *)
int MPI_Test(MPI_Request *, int* flag,  MPI_Status *
int MPI_Waitany(int count, MPI_Request *array, int *index, MPI_Status*)
int MPI_Waitall()
```

```c
// blocking and wait a message
int MPI_Probe(int src, int tag, MPI_Comm, MPI_Status *)
// check a message and return immediately 
// flag=0, if msg not arrived
int MPI_Iprobe(..., int* flag, MPI_Status*)
```

```c
// cancel a request not completed. 
// But if a communication has started, it would continue
MPI_Cancel(MPI_Request *)
MPI_Request_free()
MPI_Test_Cancelled(MPI_Status, int* flag)
```

| 函数类型     | 模式     | 阻塞型           | 非阻塞型   |
| ------------ | -------- | ---------------- | ---------- |
| 消息发送     | 标准模式 | MPI_Send         | MPI_Isend  |
|              | 缓冲模式 | MPI_BSend        | MPI_Ibsend |
|              | 同步模式 | MPI_Ssend        | MPI_Issend |
|              | 就绪模式 | MPI_Rsend        | MPI_Irsend |
| 消息接收     |          | MPI_Recv         | MPI_Irecv  |
| 消息检测     |          | MPI_Probe        | MPI_IProbe |
| 等待/查询    |          | MPI_Wait         | MPI_Test   |
| 释放通信请求 |          | MPI_Request_free |            |
| 取消通信     |          |                  | MPI_Cancel           |

### 持久通信

收发端初始化后, 使用 `MPI_Start` 激活和完成多次通信. 其他函数和单次通信相同.

```c
// initialize but not communicate, just reutrn handler of request.
MPI_Send_init(void *buf, int cnt, MPI_Datatype, int dest, 
					int tag, MPI_Comm, MPI_Request *)
MPI_Recv_init(...)

// activate persistent handler, accomplish one request
MPI_Start(MPI_Request *)
MPI_Startall(int cnt, MPI_Request* array)
```