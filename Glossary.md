# A

API: application programming interface, 应用程序编程接口

ABI: application binary interface, 应用程序二进制接口.

Assembly: 汇编

Arch: 轻量和简约的 Linux 发行版, 滚动更新.

Address Sanitizer: Google 的 C/C++ 运行时内存错误检测工具, 基于 LLVM IR 实现.

# B

BFD: binary file descriptor library, 二进制文件描述符库.

Big-endian: 大端字节序. 见 [端序](HardWare/端序.md)

Bus: 总线

Bootstrap: 自举

BSS: block started by symbol, ELF 文件中存储未初始化全局变量和局部静态变量的段. 见 [目标文件](Compiler/linking/目标文件.md)

# C

Clang: 基于 LLVM 的 C/C++/Objective-C 编译器前端. 见 [Comiler/LLVM/clang](Compiler/LLVM/clang.md)

CI/CD: continuous integration/continuous deployment, 用于自动化测试/集成/部署, 来加快
软件交互速度.

Completeness: 完备性. 逻辑系统是完备的, 如果它能证明该逻辑下的**所有**真命题.

Casting/Coercion: 显式类型转换/隐式类型转换

COFF, common object file format, ELF 格式前身. 见 [目标文件](Compiler/linking/目标文件.md)

COM, component object model, 组件对象模型.

Complilation: 编译. 见 [Compiler](Compiler/Compiler.md)

# D

DOM: 文档对象模型, 指网页文档的编程接口, 允许脚本语言 (JS) 修改网页内容结构

DNS: 域名系统, 用于将域名 (example.com) 变为关联 IP 地址.

Docker: 轻量容器化平台

Device Driver: 硬件驱动.

DDL: dynamic linking library, 动态链接库.

DSO: dynamic shared object, 动态共享对象. 

# E

ELF: executable linkable format, 可执行可连接格式. 见 [目标文件](Compiler/linking/目标文件.md)

Environment Variables: 环境变量, 指 SHELL 运行的关键共享变量.

Exit Code: 程序退出码. 

EAT: export address table, 导出地址表.

# F

Fuzzing: 高效软件模糊测试框架, 见 [Fuzzing Survey](Security/Hack/相关研究/Fuzzing%20Survey.md)

FHS: file hierarchy standard, 文件层次结构标准. 见 [系统目录](System/File%20System/系统目录.md)

Function Signature: 函数签名. 见 [compiler/linking/符号](Compiler/linking/符号.md)

Finite State Machine: 有限状态自动机. 见计算理论.

Frame Pointer: 帧指针.

# G

GUI: 计算机的用户图形界面

Git: 分布式版本控制系统, 用于跟踪文件更改和人员间协作. 和常见远程托管平台进行交互, 如: Github, GitLab

Generics: 泛型, 即允许接口同时复用于多种数据类型, 使用类型参数化实现, 在编译时确定具体数据类型(即编译器为不同数据类型的调用生成不同代码, 而不是真的动态类型). 如 CPP 模板. 

GOT: globale offset table, 全局偏移表

Grammar Parser: 语法分析器. 见 [Compiler](Compiler/Compiler.md)

# H

HTTP: Hypertext Transfer Protocol, 应用层无状态网络协议, 基于传输层 TCP 协议, 端口为 80. 

HTTPs: HTTP+SSL/TLS, 对 HTTP 协议提供安全加密.

Handle: 句柄

Heap: 堆, 数据结构. 见 [binary heap](Data%20Structure/Preliminaries/tree/binary%20heap.md)

Hook: 钩子.

# I

Image File: 映像文件.

IDE: 集成开发环境

I/O Bound: I/O 密集型.

Interrupt: 操作系统中断, 见 [操作系统概述](System/Process/操作系统概述.md)

ISR: interrupt service routine, 中断处理程序.

# J

JSON: JavaScript Object Notation, 轻量级数据交换格式, 易于解析和阅读.

# K

Kernel Mode: 操作系统内核模式.

Kali: 基于 Debina 的 Linux 发行版, 提供了内置网络安全工具.

# L

LLVM: low level virtaul machine, 编译器框架.

Lazy Binding: 延迟绑定.

Linking: 链接, 程序编译的步骤. 见 [静态链接](Compiler/linking/静态链接.md)

LSB: linux standard base.

Little-endian, 小端字节序, 见 [端序](HardWare/端序.md)

# M

MIPS: Million Instructions Per Second, 用于衡量 CPU 每秒执行指令的速度.

MMU: memory manager unit, 内存管理单元.

MSB: most significant big/byte, 最大影响位.

Multiprogramming: 多道程序技术.

Mutex: 多线程互斥量. 见 [进程与线程](System/Process/进程与线程.md)

# N

Name Mangling: 符号改编.

Namespace: 命名空间.

Northbright: 北桥, 计算机硬件架构组成部分. 见 [计算机架构](HardWare/计算机架构.md)

# O

Object File: 目标文件, 编译过程中中间文件. 见 [目标文件](Compiler/linking/目标文件.md)

ORM: Object-Relational Mapping, 软件开发概念, 将面向对象的类资源/方法和一个关系型数据库绑定起来, 对类操作即等价于对数据表操作. 类的元类等价于数据库表, 一个类实例等价于表中的一行.

OO: object-orientation, 面向对象编程.

# P

Package: 程序打包.

PAE: physical address extension. 物理地址扩展.

Page Fault: 内存缺页段错误.

Paging: 分页技术, 见 [分页技术](System/Memory/分页技术.md)

P-Code: P码, 一种编译器中间码.

PE: portable executable, windows 用可执行文件格式. 源于 COFF. 

Physical Page: 物理内存页, 也叫页框 Frame.

PIC: position-independent code, 地址无关代码.

PIE: position-independent executable, 地址无关可执行文件.

PLT: procedure linkage table. 过程链接表.

Preemption: 抢占式调度. 见 [进程调度](System/Process/进程调度.md)

Poll: 轮询结构. 并发程序模型中, 某个线程阻塞等待其他资源, 线程将不时主动查询资源是否可用来结束轮询状态.

Process: 计算机进程. 见 [进程与线程](System/Process/进程与线程.md)

Priority Schedule: 进程优先级调度. 见 [进程调度](System/Process/进程调度.md)

Round Robin: 时间片轮转法调度. 见 [进程调度](System/Process/进程调度.md)

Read-Write Lock: 读写锁. 见 [进程同步与互斥](System/Process/进程同步与互斥.md)

Reentrant: 可重入.

Relocation: 地址重定位. 

Runtime: 程序运行时.

# Q

OSI: 一种安全框架, 定义了安全攻击/安全机制/安全服务, 见 [安全模型/OSI](Security/安全模型.md)

# R

ROS: Robot Operating System, 开源机器人操作系统

Reflection: 反射, 在**运行时**操作和配置对象及其属性, 在编写代码时这些类和方法可能不是已知的, 常用于实现框架和库. 如 python 的[元类](Language/Python/运行时服务/元类.md)

RTT: 网络报文往返时间

# S

SQL: 数据库结构化查询语言, 见 [Data Storage/SQL](Data%20Structure/SQL/理论/SQL.md)

Soundness: 健全性, 一个逻辑系统是健全的, 当它所有推导都仅产生真实结论; 即不会错误地证明假命题为真.

Semantic Analyzer: 语义分析器. 见 [Compiler](Compiler/Compiler.md)

Semaphore: 信号量. 见 [进程同步与互斥](System/Process/进程同步与互斥.md)

Shared Library: 共享库

SDK: software development kit, 软件开发工具集.

Southbridge: 南桥. 见 [计算机架构](HardWare/计算机架构.md)

Stack: 先入后出的栈数据结构, 见 [kernel/list](Data%20Structure/Preliminaries/linux%20kernel/list.md). 也指进程内存空间的一种结构, 见 [linux 内存空间分布](System/Memory/linux%20内存空间分布.md)

Static Shared Library: 静态共享库.

Symbol Link: 软链接.

Symbol Resolution: 符号决议.

Synchronization: 进程间同步, 见 [进程同步与互斥](System/Process/进程同步与互斥.md)

Syntax Tree: 语法树, 编译前端的中间产物, 见 [Compiler](Compiler/Compiler.md)

System Call: 系统调用. 见 [操作系统概述](System/Process/操作系统概述.md)

# T

TagDispatch: 一种函数反射方法, 见 [运行时调用选择](Language/C++/语法/运行时调用选择.md#函数体)

Time-Sharing System: 分时系统.

Time Slice: 时间片.

Thread: 线程, 一种更轻量进程, 见 [System/进程与线程](System/Process/进程与线程.md)

Token: 词元.

# U

URL: 互联网统一资源定位符

Ubuntu: 基于 Debian 的开源 Linux 流行发行版.

User Mode: 操作系统用户空间.

# V

Virtual Address Space: 虚拟地址空间. 一种管理内存的方式.

VMA: virtual Memory Area, 代指进程拥有的某片连续虚拟内存区域. 见 [linux 进程与内存管理数据结构](System/Process/linux%20进程内存管理结构.canvas) 的 `vm_area_struct` 结构.

# W

WSL: Windows Subsystem for Linux, 见 [OS/Linux Distribution/WSL](System/Distributions/WSL/配置%20-%20从此开始.md)

# X

# Y

YAML, 类似 JSON 的数据交换格式, 但语法更复杂, 由多层嵌套键值对等组成.

# Z