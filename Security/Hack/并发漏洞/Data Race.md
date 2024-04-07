## Data Race

file discriptor:

```cpp 
int fd = open(...);

// Thread 1.
write(fd, ...);

// Thread 2.
close(fd);
```

non-atomic operations, may occur in gc.

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

race on a bit field, like x is `struct{int a:4, b:4;}`

```cpp
void Thread1() {
  x.a++;
}

void Thread2() {
  x.b++;
}
```

## Use Bool Flag instead of Mutex Signal

```cpp
bool done = false;

void Thread1() {
  while (!done) {
    do_something_useful_in_a_loop_1();
  } 
  do_thread1_cleanup();
}

void Thread2() {
  do_something_useful_2();
  done = true;
  do_thread2_cleanup();
}
```

On x86, the biggest issue is the compile-time optimizations.  

- Part of the code of `do_something_useful_2()` can be moved below `done = true` by the compiler.
- Part of the code of `do_thread2_cleanup()` can be moved above `done = true` by the compiler.
- If `do_something_useful_in_a_loop_1()` doesn't modify "done", the compiler may re-write Thread1 in the following way:

```cpp
  if (!done) {
    while(true) {
      do_something_useful_in_a_loop_1();
    } 
  }
  do_thread1_cleanup();
```

**use lock instead of bool**

## Double Init or UnAtomic Init

Unatomic init: strange compiler behavior. True construction of obj(MyType) may happen after `obj==NULL`, and then obj is indeed not a nullptr but not initialized completely.

```cpp
MyType* obj = NULL;

void Thread1() {
  obj = new MyType();
}

void Thread2() {
  while(obj == NULL)
    yield();
  obj->DoSomething();
}
```

double init:

```cpp
static MyObj *obj = NULL;

void InitObj() { 
  if (!obj) 
    obj = new MyObj(); 
}

void Thread1() {
  InitObj();
}

void Thread2() {
  InitObj();
}
```

## Updates under ReaderLock

Updates happening under a reader lock.

```cpp
void Thread1() {
  mu.ReaderLock();
  var++;
  mu.ReaderUnlock();
}

void Thread2() {
  mu.ReaderLock();
  var++;
  mu.ReaderUnlock();
}
```

## Doubled-checked locking

```cpp
bool inited = false;
void Init() {
  // May be called by multiple threads.
  if (!inited) {
    mu.Lock();
    if (!inited) {
      // .. initialize something
    }
    inited = true;
    mu.Unlock();
  }
}
```

## Race during destruction

Sometimes objects are created on stack, passed to another thread and then destroyed without waiting for the second thread to finish its work. Often happens in ROS2.

```cpp
void Thread1() {
  SomeType object;
  ExecuteCallbackInThread2(
    SomeCallback, &object);
  ...
  // "object" is destroyed when
  // leaving its scope.
}
```

## Race on free

ASan can detect these bugs.

```cpp
int *array;
void Thread1() {
  array[10]++;
}

void Thread2() {
  free(array);
}
```

## Race during exit.

If a program calls `exit()` while other threads are still running, static objects may be destructed by one thread and used by another thread at the same time

```cpp
#include "pthread.h"
#include <map>

static std::map<int, int> my_map;

void *MyThread(void *) {  // runs in a separate thread.
  int i = 0;
  while(1) {
    my_map[(i++) % 1000]++;
  }
  return NULL;
}

int main() {
  pthread_t t;
  pthread_create(&t, 0, MyThread, 0);
  return 0;
  // exit() is called, my_map is destructed
}
```

## Race on a mutex

Destructor of `MutexLock` can unlock already deleted mutex, if `UserCompletionCallback` is already started executing on another thread.

```cpp
class Foo {
  Mutex m_;
  ...

 public:
  // Asynchronous completion notification.
  void OnDone(Callback *c) {
    MutexLock l(&m_);
    // handle completion
    // ...
    // schedule user completion callback
    ThreadPool::Schedule(c);
  }
  ...
};

void UserCompletionCallback(Foo *f) {
  delete f;  // don't need it anymore
  // notify another thread about completion
  // ...
}
```

## Race on vptr during construction

初始化 Derived 时, 在基类 Base 初始化过程中 PointA 处线程 `forEach()` 被抢占, 此时 Derived 的虚函数 `Execute()` 尚未注册, 导致调用的实际是 Base 的 `Execute()`

```cpp
class Base {
  Base() {
   global_mutex.Lock();
   global_list.push_back(this);
   global_mutex.Unlock();
   // point (A), see below
  }
  virtual void Execute() = 0;
  ...
};

class Derived : Base {
  Derived() {
    name_ = ...;
  }
  virtual void Execute() {
    // use name_;
  }
  string name_;
  ...
};

Mutex global_mutex;
vector<Base*> global_list;

// Executed by a background thread.
void ForEach() {
  global_mutex.Lock();
  for (size_t i = 0; i < global_list.size(); i++)
    global_list[i]->Execute()
  global_mutex.Unlock();
}
```

> reference:   
> [ThreadSanitizerPopularDataRaces · googlesanitizers Wiki](https://github.com/google/sanitizers/wiki/ThreadSanitizerPopularDataRaces)