### 编译动态库

`-fPIC`: 生成"位置无关代码" (Position-Independent Code), 即代码可以被加载到任何内存位置上运行, 而不是在某一固定的内存地址. 有利于程序动态链接.

`-shared` 指示编译器生成动态链接库 (如 `.so`)

### llvm-confg

快捷获取使用 llvm 相关工具时所需要的编译参数.

```shell
clang++ -shared -fPIC -o hello.so hello.cpp \
	$(llvm-config --cxxflags) \
	$(llvm-config --ldflags) \
	$(llvm-config --libs) \
	$(llvm-config --system-libs)
```