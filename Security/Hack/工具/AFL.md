## bitmap

AFL 中, 统计edge的覆盖率信息. 考虑如下基本块路径:
```
A
|\
| \
|  B
| /
|/
C
```

如果A可以直接到C, 也可以经过B再到C. 那么仅统计ABC**基本块覆盖率**时, 即使ABC都被覆盖, 也不能说明AC路径被执行. 这样的控制流被称为"critical".

SanCov 和 AFL 提供了 edge 插桩功能: 将一个虚拟块 D 插到 critical edge 上.
```
A
|\
| \
D  B
| /
|/
C
```

wiki上说, branch coverage 是 edge coverage 的一个子集. 网上信息很少, 姑且认为 br cov 和 1edge cov 等价, llvm 使用的是 1edge cov, llvm bb cov 指的是 node cov. 多边缘覆盖检测, 其实有点像路径覆盖检测了.

**AFL 采用边缘覆盖率检测, 为每个 bb (基本块) 用编译时随机数编号, 然后用哈希函数 `hash=(bb1>>1)^bb2` 的结果表示两个基本块之间的 edge, 记录在 bitmap 中.**

## Fork Server

```c
#include ...

void forkserver_loop() {
	while (1) {
		wait_for_pipe(); // 等待 fuzzer 发送指令
		pid_t pid = fork();

		if (pid > 0) {
			// 父进程, 将子进程 pid 发送给 fuzzer
			write_pipe(pid);
		} else {
			// 子进程, 跳出函数执行原有代码
			return;
		}
	}
}

int main() {
	forkserver_loop(); // 循环等待
	/* 程序原有代码 */
}
```