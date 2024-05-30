OpenMP (Open Multi-Processing) 是一套支持跨平台共享内存方式的多线程并发的编程API, 使用 `#pragma omp` 等编译器预处理指令来控制线程. OpenMP 使用 fork-join 并行模式, 从主线程分离出一系列线程完成任务, 然后所有线程完成任务汇合至主线程, 多线程部分被称为 **并行域**.

编译时链接 OenMP 库: `-fopenmp`

OpenMP 基本指令格式为 `#pragma omp directives [clause ...]`

![|500](../../../attach/Pasted%20image%2020240311161748.png)

## OpenMP Directives (指令)

#### parallel 指令

```cpp
#pragma omp parallel
{
	int i = omp_get_thread_num();
	
	// 多个连续 cout 不是线程安全的, 可能有输出混乱, 只能使用 printf
	printf("Hello from thread %d\n", i);
}
```

结果:
```
Hello from thread 6
Hello from thread 3
....
```

#### for 指令

```cpp
// omp for 仅在一个线程内运行
#pragma omp for
for (int i=0; i<4; ++i)
	printf("%d ", i);

// parallel 开启多线程, 下列程序等价于 omp parallel for
#pragma omp parallel
{
	#pragma omp for
	for (int i=0; i<4; ++i)
		printf("%d ", i);
}
```

parallel for 循环成功并行需要满足以下条件:
1. for 第一条语句必须是循环变量初始化语句, 如 `i=0`
2. for 第二条语句必须是 `i<n, i<=n, i>n, i>=n` 其一. 且循环中不能有直接影响循环次数的语句, 如 `break` 或直接修改 `i`
3. for 第三条语句必须是 `i++, ++i, i--, --i, i+=n, i-=n, i=i+n, i=i-n, i=n+i` 其一

另外, 在并行域内声明的变量是**线程私有的**, 在并行域外声明的变量则是**线程共享的**, 线程共享变量可能会产生数据竞争!

#### section 指令

指明在多个线程 (section) 中执行, `section` 中代码不会被多个线程执行 (而是分配给一个线程.)

```cpp
#pragma omp parallel sections
{
	#pragma omp section
	printf("section1, thread id is %d\n", omp_get_thread_num());

	#pragma omp section
	printf("section2, thread id is %d\n", omp_get_thread_num());

	#pragma omp section
	printf("section3, thread id is %d\n", omp_get_thread_num());

	#pragma omp section
	printf("section4, thread id is %d\n", omp_get_thread_num());
}

// output, order is random 
section3, thread id is 5
section1, thread id is 6
section2, thread id is 1
section4, thread id is 7
```

由于默认使用8个线程并行执行, 所以此时4个编号是0-7间随机数. 要输出 `0-3`, 见后述 `private` 子句.

#### critical 指令

用于指定一个代码块为临界区, 该区域同一时间只有一个线程可以访问.

```cpp
int sum = 0;
#pragma omp parallel for
for (int i = 0; i < N; i++) {
    #pragma omp critical
    {
        sum += some_function(i); // 其实使用归约 (reduction) 更好
    }
}
```

可以给临界区命名 `#pragma omp critical [name]`, 默认所有匿名临界区是同一个临界区, 同一临界区会**互相阻塞**.

#### single 指令

用于指定该代码块只有**一个**线程执行, 其他线程会阻塞等待其结束 (使用 `nowait` 子句可以让其他线程非阻塞.)

```cpp
#pragma omp parallel
{
	#pragma omp single [nowait]
	{
		"single block"
		"wait..."
		sleep(3);
	}
	"parralel block."
}
```

#### master 指令

`#pragma omp master newline` 指定代码块仅主线程执行

#### atomic 指令

指定特定变量或操作被原子化访问更新.

```cpp
#pragma omp parallel {
	for (i=0; i<1000; i++){
		sleep(1);
		#pragma omp atomic
		count++;
	}
}
```

#### barrier 指令

创建一个同步点, 程序会等待所有线程都执行到该点, 然后才继续执行后续代码.

```cpp
#pragma omp parallel
{
    // part one
  
    #pragma omp barrier

    // part two
}
```

## OpenMP Clause (子句)

#### if 子句

根据条件决定是否并行执行

```cpp
#define DATA_SIZE 100

#pragma omp parallel if(DATA_SIZE > 1000)
for (int i=0; i<DATA_SIZE; i++)
	process_data(i);
```

#### num_threads 子句

用于直接指定并行的线程数量

```cpp
#pragma omp parallel sections num_threads(4)
{
	#pragma omp section
	printf("section1, thread id is %d\n", omp_get_thread_num());

	#pragma omp section
	printf("section2, thread id is %d\n", omp_get_thread_num());

	#pragma omp section
	printf("section3, thread id is %d\n", omp_get_thread_num());

	#pragma omp section
	printf("section4, thread id is %d\n", omp_get_thread_num());
}

// output, order is random 
section4, thread id is 3
section3, thread id is 2
section2, thread id is 0
section1, thread id is 1
```

#### private/shared 子句

并行域内的变量声明会自动成为线程私有变量. 对于外部变量, 使用 `private` 将其在每个线程中拷贝独立的副本. 设立独立变量的方法, 比临界区方法效率高. **注意, 声明为 `private` 的变量不继承并行域外的主线程初始值, 实际是不同变量.**

```cpp
int temp; // 域外变量
#pragma omp parallel for private(temp) 
for(i = 0; i < N; i++) 
{ 
	temp = arr[i] * 2; // 线程对temp的修改不影响其他线程 
	arr[i] = temp; 
}
```

使用 `private` 时要注意线程总数, 下面程序中 `k` 对线程中是私有的, 但是对不同轮次迭代不是私有的; 即一个线程可能执行多轮迭代, 这些轮迭代中 `k` 是持续的.

```cpp
omp_set_num_threads(2);
int k=10
#pragma omp parallel for firstprivate(k) lastprivate(k)
for(int i = 0; i < 4; i++){
	k=k+i;
	printf("ID=%d, k=%d\n", omp_get_thread_num(), k);
}
printf("last k=%d\n", k);
return 0; 

/* output:
ID=0, k=100
ID=0, k=101
ID=1, k=102
ID=1, k=105
last k=105 
*/
```

`fristprivate(temp)` 指定变量为私有副本, 同时继承主线程中的初始值 (即原共享变量).

`shared(temp)` 共享变量 (默认)

`lastprivate(temp)` 退出并行区域时, 将最后一次for迭代私有变量值赋值给共享变量.

`threadprivate(temp)` 指定变量为线程的全局静态私有副本, 可能跨多个并行区域, 皆共享该变量.

#### reduction 子句

归约操作对象是一个/多个变量+操作符, 每个线程将创建该变量的一个私有副本. 在并行域结束处, 使用归约操作符对各个线程的私有副本进行归约.

```cpp
int sum = 0;
#pragma omp parallel for reduction(+:sum)
for (int i = 0; i < N; i++) 
	sum += some_function(i); // 归约自动避免了共享变量的竞争
```

OpenMP 支持 8 种 reduction 操作符, 每个线程创建私有变量时初始值确定如下. 要保证计算结果和串行结果一致, 运算必须满足*交换律和结合律*. ~~减法不满足上述性质, 但是在特殊情况也可以归约.~~

| 操作符 | `+` | `-` | `*` | `&` | \| | `^` | `&&` | \|\| |
| ------ | --- | --- | --- | --- | --- | --- | ---- | ---- |
| 初始值 | 0   | 0   | 1   | ~0  | 0   | 0   | 1    | 0     |

## OpenMP API

`<omp.h>` 中常见 api 列表:

```cpp
// 设置并行区域线程数
void omp_set_num_threads(int _Num_threads);

// 返回当前线程数目，在串行代码中调用将返回1
int omp_get_num_threads(void);

// 返回程序的最大可用线程数量
int omp_get_max_threads(void);

// 返回当前线程id，从0开始编号
int omp_get_thread_num(void);

// 返回程序可用的处理器数
int omp_get_num_procs(void);

// 启用或禁用可用线程数的动态调整(缺省情况下启用动态调整)。
void omp_set_dynamic(int _Dynamic_threads);

// 确定在程序中此处是否启用了动态线程调整。
int omp_get_dynamic(void);

// 确定线程是否在并行区域的动态范围内执行
int omp_in_parallel(void);

// 启用或禁用嵌套并行操作
void omp_set_nested(int _Nested);

// 确定在程序中此处是否启用了嵌套并行操作
int omp_get_nested(void);


// 初始化一个（嵌套）互斥锁
void omp_init_lock(omp_lock_t * _Lock);
void omp_init_nest_lock(omp_nest_lock_t * _Lock);

// 销毁一个（嵌套）互斥锁并释放内存
void omp_destroy_lock(omp_lock_t * _Lock);
void omp_destroy_nest_lock(omp_nest_lock_t * _Lock);

// 获得一个（嵌套）互斥锁
void omp_set_lock(omp_lock_t * _Lock);
void omp_set_nest_lock(omp_nest_lock_t * _Lock);

// 释放一个（嵌套）互斥锁
void omp_unset_lock(omp_lock_t * _Lock);
void omp_unset_nest_lock(omp_nest_lock_t * _Lock);

// 试图获得一个（嵌套）互斥锁，并在成功时放回真，失败时返回假
int omp_test_lock(omp_lock_t * _Lock);
int omp_test_nest_lock(omp_nest_lock_t * _Lock);

// 从过去的某一时刻经历的时间，一般用于成对出现，进行时间比较
double omp_get_wtime(void);

// 得到clock ticks的秒数
double omp_get_wtick(void);
```

## 参考:  

[OpenMP教程  wjin](https://w-jin.github.io/tech/openmp/)  

[官方白皮书: *OpenMP Application Programming Interface*](https://www.openmp.org/wp-content/uploads/OpenMP-API-Specification-5.0.pdf)