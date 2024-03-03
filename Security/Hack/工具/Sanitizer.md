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

### use

use `clang` to compile and link 
- `-fsanitize=address` enable ASan
- `-01` optimize code performance
- `-fno-omit-frame-pointer` get nicer stack traces in error msgs.

### principle

The run-time library replaces the `malloc` and `free` functions. The memory around malloc-ed regions (red zones) is poisoned. The `free`\-ed memory is placed in quarantine and also poisoned. Every memory access in the program is transformed by the compiler in the following way:

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

## LSan

