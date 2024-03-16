- Address Sanitizer: `libasan5`
- Leak Sanitizer: `liblsan0`
- Undefined Behavior Snitizer: `libubsan1`

## MSan 

`MemorySanitizer` (aka. MSan) detects uninitialized memory reads for c.

Uninitialized values occur when stack- or heap-allocated memory is read before written. 

MSan detects cases where uninitialized values affect program execution, which means MSan tracks the spread of uninitialized data in memory (in *bit-exact*), and reports a warning when a code branch is taken (or not) depending on such data, otherwise keeps silent.

**use**: `-fsanitize=memory -fPIE -pie`
- `-fno-omit-frame-pointer`, get stack traces.
- `-fsanitize-memory-track-origins`, tracks the original allocation of uninitialized data with additional 1.5x-2.5x slowdown.

## TSan

`ThreadSanitizer` (aka. TSan) detects data-race bugs for c. **Data race** , which is banned by C++11 as *Undefined Behavior*,  occurs when two threads access the same variable concurrently and at least one of the accesses is write.

*use*: compile with flags `-fsanitizer=thread`.
- `-o2` optimize code performance.
- `-g` get file names and line numbers in the warning msgs.

详见 [Thread Sanitizer](Thread%20Sanitizer.md)

## UBSan

UndefinedBehaviorSanitizer

## Asan

AddressSanitizer (Aka. ASan) detects bugs for C++ like:
- **Use after free** (dangling pointer dereference)
- **Heap/Stack/Global buffer overflow**
- Use after return, due to error scope. with flags: `ASAN_OPTIONS=detect_stack_use_after_return=1`.
- **Use after scope**. with flags: `-fsanitize-address-use-after-scope`
- Initialization order bugs
- Memory leaks

### how to use

use `clang` to compile and link 
- `-fsanitize=address` enable ASan
- `-01` optimize code performance
- `-fno-omit-frame-pointer` get nicer stack traces in error msgs.

### principle

ASan 运行时库替换了 `malloc()` and `free()` 函数. 被 malloc 的常规内存区域两侧的内存被毒化 (red zones is poisoned), 用于检测数组越界; 被 free 的内存实际上并没有被释放而是全部被毒化, 每次内存访问时检查是否访问了被毒化区域.

Before:

```
*address = ...;  // or: ... = *address;
```

After:

```
if (IsPoisoned(address)) {
  ReportError(address, kAccessSize, kIsWrite);
}
*address = ...;  // or: ... = *address;
```

**毒化 (Poison) 实际上意味着该内存区域对应的 Shadow 内存区域被写上特殊值, 意味着此内存区域不再可访问 (不安全).**

ASan 维护了一个 Shadow Table 的内存区域, 将**内存访问状态**映射到其上 `shadow=(addr>>3)+offset`. Asan 还维护了 Shadow Gap 区域, 用于隔离 Shadow 部分内存, 防止恶意软件或无意访问 Shadow 内存 (类似防御 BufferOverflow 的 Canary).

```cpp
byte* MemToShadow(address){
	return (address>>3) + offset
}

shadow_address = MemToShadow(address);
if (ShadowIsPoisoned(shadow_address)) {
  ReportError(address, kAccessSize, kIsWrite);
}
```

实现原理详见 [Address Sanitizer](Address%20Sanitizer.md), 核心技巧就是如何高效实现 `IsPoisoned()`. [官方wiki](https://github.com/google/sanitizers/wiki/AddressSanitizerAlgorithm)

#### Mapping

为了节省空间, ASan 将常规内存 8bytes (one qword) 映射为 Shadow 区域 1byte, 来监控对内存的访问. qword 有如下几种值:
- `0`: 表示该 8bytes 未被污染, 可访问. 这是最常见的, 意味着内存安全.
- 负数: 表示 8bytes 被完全污染, 即皆不可访问.
- 前 k bytes 未被污染, 8-k bytes 被污染. 即 malloc 的字节数并不是 8bytes 对齐时, ASan 会将未对齐的剩余部分污染. (编译器也会做这种对齐优化, 这部分通常不会被使用).

```cpp
byte *shadow_address = MemToShadow(address);
byte shadow_value = *shadow_address;
if (shadow_value) {
  if (SlowPathCheck(shadow_value, address, kAccessSize)) {
    ReportError(address, kAccessSize, kIsWrite);
  }
}

// Check the cases where we access first k bytes of the qword
// and these k bytes are unpoisoned.
bool SlowPathCheck(shadow_value, address, kAccessSize) {
  last_accessed_byte = (address & 7) + kAccessSize - 1;
  return (last_accessed_byte >= shadow_value);
}
```
> 如果偏移量足够大, 超过被毒化的范围, 是不是就检查不出越界了? 确实..

#### Report Error

ASan 抛出异常时, 需要尽量做到紧凑, 确保程序崩溃时也能尽量获取到准确报告. (不过 ASan 也支持不强制终止程序的报告方式.)

1. 将错误地址拷贝到 `%rax` 寄存器
2. 执行 `ud2`, Undefined Instruction, 会导致程序抛出 `SIGILL`, 强制触发一个可捕获的异常, 触发错误处理流程.
3. 在接下来的指令中, 用 1byte 大小数据来告知*检测到的错误类型和内存体积*

整个 `ReportError()` 调用仅有 3 条指令, 5-6bytes 数据.

#### Stack

为检测 BufferOverflow, Asan 会修改栈结构为:

```cpp
// original code
void foo() {
  char a[8];
  ...
  return;
}
```

```cpp
// code after asan
void foo() {
  char redzone1[32];  // 32-byte aligned
  char a[8];          // 32-byte aligned
  char redzone2[24];
  char redzone3[32];  // 32-byte aligned
  int  *shadow_base = MemToShadow(redzone1);
  shadow_base[0] = 0xffffffff;  // poison redzone1
  shadow_base[1] = 0xffffff00;  // poison redzone2, unpoison 'a'
  shadow_base[2] = 0xffffffff;  // poison redzone3
  ...
  shadow_base[0] = shadow_base[1] = shadow_base[2] = 0; // unpoison all
  return;
}
```

## LSan

## Sanitizer Coverage

Sanitizer 自带的覆盖率统计工具, 见:
- [Sanitizer Coverage](Sanitizer%20Coverage.md)
- [AFL](AFL.md)
- [Coverage](Coverage.md)

