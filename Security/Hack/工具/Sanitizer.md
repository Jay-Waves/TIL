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

### popular data-race cases

#### 1 Simple race

**Thread-hostile reference counting**, 引用计数时, 没有互斥访问.

```cpp
// Ref() and Unref() may be called from several threads.
// Last Unref() destroys the object.
class RefCountedObject {
 ...
 public:
  void Ref() {
    ref_++;  // Bug!
  }
  void Unref() {
    if (--ref_ == 0)  // Bug! Need to use atomic decrement!
      delete this;
  }
 private:
  int ref_;
};
```

#### 2 Race on a complex object

two threads access a non-thread-safe complex object (e.g. an STL container) without synchronization.

```cpp
std::map<int,int> m;

void Thread1() {
  m[123] = 1;
}

void Thread2() {
  m[345] = 0;
}
```

#### 3 Publishing objects without synchronization

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

ASan 运行时库替换了 `malloc()` and `free()` 函数. 被 malloc 的常规内存区域两侧的内存被毒化 (red zones is poisoned), 用于检测数组越界, 这部分区域也被称为 Shadow; 被 free 的内存实际上并没有被释放而是全部被毒化, ASan 会在每次内存访问时检查是否有对被毒化区域访问的行为.

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

实现原理详见 [Address Sanitizer](Address%20Sanitizer.md), 核心技巧就是如何高效实现 `IsPoisoned()`. [官方wiki](https://github.com/google/sanitizers/wiki/AddressSanitizerAlgorithm)

#### Mapping

ASan 将常规内存 8 字节映射为阴影区域 1 字节.

> 如果偏移量足够大, 超过被毒化的范围, 是不是就检查不出越界了? 确实..

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
- [Coverage](../Coverage.md)

