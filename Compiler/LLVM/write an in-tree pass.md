
# Pass in Src Tree

**[using new pass manager](use%20new%20pass%20manager.md)**

## build your llvm

```shell
cmake ../llvm -G "Unix Makefiles" \
	-DLLVM_ENABLE_PROJECTS="clang;compiler-rt" \
	-DCMAKE_BUILD_TYPE=Release \
	-DLLVM_TARGETS_TO_BUILD="X86"

make -j4
```

## add file to cmake

在 `llvm/lib/Transforms/Utils` 下活动, 修改 `.../Utils/CMakeLists.txt`

## create pass

```cpp
//../Utils/HelloWorld.h: 

#ifndef LLVM_TRANSFORMS_HELLONEW_HELLOWORLD_H
#define LLVM_TRANSFORMS_HELLONEW_HELLOWORLD_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class HelloWorldPass : public PassInfoMixin<HelloWorldPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_HELLONEW_HELLOWORLD_H
```

```cpp
// ../Utils/HelloWorld.cpp:
#include "llvm/Transforms/Utils/HelloWorld.h"

using namespace llvm;

PreservedAnalyses HelloWorldPass::run(Function &F,
                                      FunctionAnalysisManager &AM) {
  errs() << F.getName() << "\n";
  return PreservedAnalyses::all();
}
```

## register pass

在 `llvm/lib/Passes/PassRegistry.def` 添加: `FUNCTION_PASS("helloworld", HelloWorldPass())`

在 `llvm/lib/Passes/PassBuilder.cpp` 添加: `#include "llvm/Transforms/Utils/Helloworld.h"`

## run our pass with opt

```shell 
cmake --build build/ --target opt

build/bin/opt -disable-output /tmp/a.ll -passes=helloworld
```

# register pass as plugins

在 llvm-project 目录创建一个 CMake 项目, CMakeLists.txt 如下:

```cmake
add_llvm_pass_plugin(MyPass, source.cpp)
```

此 Pass 必须向新 PassManager 提供下列两个中至少一个入口:
1. 用于静态注册的入口点, 需要设置 `LLVM_${NAME}_LINK_INTO_TOOLS=ON`, 集成入 LLVM
```cpp
llvm:PassPluginLibraryInfo get##Name##PluginInfo();
```
2. 用于动态加载的入口点, 例子详见 `llvm/examples/Bye`
```cpp
extern "C" ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() LLVM_ATTRIBUTE_WEAK;
```

> 详见 https://llvm.org/docs/WritingAnLLVMNewPMPass.html