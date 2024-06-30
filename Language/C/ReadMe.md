## 何为 C 标准?

libc 指 C 语言标准库, libc++ 指 C++ 标准库. 语言委员会提供一套标准, 不同编译器和操作系统都会有独立的实现. 

因为 C 语言是系统级语言, 除了通用标准库 libc 外, 编译器还需要实现大量系统调用. 如在类 Unix 系统上 (BSD, Linux, MacOS), 常使用 POSIX 标准定义的 API; 在 Windows 上, 则使用 Windows API.

| 操作系统                       | 编译器     | C 标准实现 | C++ 标准实现 | 解释                                                                      |
| ------------------------------ | ---------- | ---------- | ------------ | ------------------------------------------------------------------------- |
| GNU/Linux                      | GCC        | glibc      | libstdc++    |                                                                           |
| GNU/Linux                      | Clang/LLVM | llvm-libc[^2]  | libc++       | 较旧版本的 LLVM 仍使用 glibc, 而 libc++ 是 LLVM 独立实现的现代 C++ 标准库 |
| BSD (FreeBSD, NetBSD, OpenBSD) |            | BSD libc   |              |                                                                           |
| macOS, iOS                     | Clang/LLVM | Apple libc | libc++       | macOS 前身基于 BSD, 因而 Apple libc 基于 BSD libc                            |
| Windows                        | Clang/LLVM | MSCVrt [^3]      | libc++             | 一套标准实现就支持*多平台*是 LLVM 的设计目标                                             |
| Windows                        | GCC/MinGW  | MSVCrt     | MSVC++             | 借助 MinGW 模拟 GNU[^1]                                                   |
| Windows                        | MSVC       | MSVCrt     | MSVC++       | Microsoft Visual C, 原生库和集成环境                                      |
| 嵌入式系统                     |            | newlib     |              |                                                                           |

[^1]: 注意: MinGW  (Minimalist GNU for Windows) 并不能完全移植 Unix-like 系统上的 C 程序到 Windows. MinGW 仍使用 Windows 系统 API, 而仅**部分**兼容 POSIX API, 其旨在提供类 GNU 的开发链条, 方便开箱即用, 但是不适合大型程序. 如果需要兼容完整 POSIX 环境, 即提供完整 POSIX API 到 Windows API 的翻译, 可以用 **Cygwin**.

[^2]: llvm-libc 正在开发中, 不稳定. 所以现在在 windows 上用不了.

[^3]: MSVCrt (microsoft visual c runtime) 指 `msvcrt.dll`, 是 Windwos 系统对 libc 的实现. 在 Windows10 之后, 引入了 UCRT 的现代版本 libc 实现, 提供了对现代 C 标准更好的支持. 都不开源...


| 系统调用标准                                    | 维护组织           | 解释                                       | 平台支持      |
| ------------------------------------------- | ------------------ | ------------------------------------------ | ------------- |
| POSIX (POSIX Operating System API)                                     | IEEE               |                                            | Unix-like     |
| Windows API                                 | Microsoft          |                                            | Windows       |
| LSB (Linux Standard Base)                   | Linux 基金会       | 提高 Linux 不同发行版之间的兼容性          | Linux         |
| SVID (System V Interface Definition)        | AT&T, Unix SystemV | 不常见, 仅兼容[^5]            | Unix SystemV |
| RTOS (Real-teim Operating System Standards) |                    | 在嵌入式系统中, 用于提供更低延迟和更高确定性 |               |
| XPG, SUS                                     | The Open Group     |   不常见                                         | Unix[^4]          |

[^4]: SUS 和 XPG 都是针对**正式 UNIX** 系统的, 而不是类 Unix 系统. 
[^5]: System V 是 AT&T 开发的 UNIX 版本, 其 System V IPC (跨进程通信) 和 SystemV Init (系统进程启动和管理) 接口影响广泛. 但现在影响力已较弱, 如 SysV IPC 被 POSIX IPC 取代, SysV Init 被 systemd 取代.

## 参考

https://users.cs.cf.ac.uk/Dave.Marshall/C/. Dave Marshall. 1999-03.