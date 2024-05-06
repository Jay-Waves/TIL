Boost.Process is a library to manage system processes. Used to:

- create child processes
- setup streams for child processes
- communicate with child processes through streams (synchronously or asynchronously)
- wait for processes to exit (synchronously or asynchronously)
- terminate processes

```cpp
namespace bp=boost::process 

// 标准库
int result = std::system('g++ main.cpp');

// bp
int result = bp::system('g++ main.cpp'); 


/* launch */

bp::spawn(bp::search_path("chrome"), "www.boost.org"); //spawn 不会等待信号值返回, 直接分离进程.

bp::child c(bp::search_path("g++"), "main.cp");

while(c.running())
	do_something();

c.wait(); // 也有 wait_for, wait_until

int result = c.exit_code();


/* 处理错误 */

std::error_code ec;
bp:system('g++ main.cpp', ec);


/* 同步 IO */

bp::system('g++ main.cpp', bp::std_out > stdout, bp:std_err > stderr, bp::std_in < stdin);
bp:system("g++ main.cpp", bp:std_out > bp::null) // 丢弃 output
bp::system("g++ main.cpp", bp::std_out > "gcc_out.log") // 输出到文件


/* 异步 IO, 使用 asio 库 */
boost::asio::io_service ios;
std::vector<char> buf(4096);
bp::child c(bp::search_path("g++"), "main.cpp", bp::std_out > boost::asio::buffer(buf), ios);
ios.run(); // asio 在后台异步运行程序
int result = c.exit_code();

/* 进程分组, 管理多个子进程 (以及孙子进程) */
bp::group g;
bp::spawn("make", g); // make 会生成很多编译子进程
bp::spawn("bar", g);
// do something
g.wait(); // or g.terminate()

```

## Pipes

pipestream: `ipsrteam`, `opstream`, `pstream`.

#### Named Pipes

#### Anonymous Pipes

## Processes

返回值 0 代表成功. POSIX 系统定义了其他返回值和主动操作信号量, Windows没有, 由于 Boost 是跨平台库, 所以也不支持这些.