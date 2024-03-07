## libasan库缺失

```shell
$ g++ -O -g -fsanitize=address heap-use-after-free.cpp

/usr/bin/ld: cannot find /usr/lib64/libasan.so.0.0.0
collect2: error: ld returned 1 exit status
```

即链接过程没有找到 libasan 库, asan 运行时函数是封装在 Compiler-RT 库中的, 所以一般会在 `/usr/lib` 中出现. ps: gcc 默认是使用动态链接库来使用 asan, clang 则默认静态.

解决办法是: `sudo apt install libasan`; 或重新编译 `llvm-project/compiler-rt`, 然后 `make install`

## x64_ASAN 不兼容

64位平台可能出现以下错误, 参考[issue](https://stackoverflow.com/questions/59007118/how-to-enable-address-sanitizer-at-godbolt-org/59010436#59010436)

```
==3==ERROR: AddressSanitizer failed to allocate 0xdfff0001000 (15392894357504) bytes at address 2008fff7000 (errno: 12)
==3==ReserveShadowMemoryRange failed while trying to map 0xdfff0001000 bytes. Perhaps you're using ulimit -v
```

程序开启 64 位 ASAN 后, 初始化时会声明 20TB 内存 (即尽可能大). 开启虚拟内存 (即 swap) 后一般可以正常运行, 否则就只能直接限制其内存的分配大小.

## 主程序和动态链接的 ASAN 兼容否?