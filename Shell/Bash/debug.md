## Debug

### network debug

see [Bash/network](网络.md)

### `ldd`

列出程序所依赖的共享库文件 (.so)

<pre>
$ ldd /usr/bin/clang
 linux-vdso.so.1 (0x00007fffc04c3000)
 libclang-cpp.so.14 => /lib/x86_64-linux-gnu/libclang-cpp.so.14 (0x00007f8e58795000)
 libLLVM-14.so.1 => /lib/x86_64-linux-gnu/libLLVM-14.so.1 (0x00007f8e51ec3000)
 libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f8e51c97000)
 libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f8e51bb0000)
 #  [库名] => [地址] (加载到的内存地址)
 </pre>

ldd 存在[严重安全漏洞](https://catonmat.net/ldd-arbitrary-code-execution), 确保目标程序是受信任文件.

### `time`

```
$ time ./lab2
./lab2  0.59s user 0.01s system 19% cpu 3.008 total
```

### `dmesg`

引导及系统错误信息

### `strace`, `ltrace`

用于审查程序失败/挂起/崩溃的原因, 或者大致了解程序性能.  strace 用于跟踪系统调用, `ltrace` 用于跟踪程序对动态库函数的调用 (也包括标准库函数 `malloc`, `printf` 等)

- `-c`: 开启 profile 性能分析, `strace -c ./my_program`
- `-p`: 附加到一个运行中的进程, `strace -p 1234`

### `nm`

列出目标对象的**符号表内容**, `nm filename`

- `-l` 列出行号信息
- `-n` 按地址排序

### `stap`, `perf`, `sysdig`

更深层的系统分析: `stap` ([SystemTap](https://sourceware.org/systemtap/wiki)), [`perf`](https://en.wikipedia.org/wiki/Perf_(Linux)), 以及[`sysdig`](https://github.com/draios/sysdig)=strace+tcpdump+htop.
