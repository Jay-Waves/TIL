
CMake 没有为 `LLVM bitcode` 提供直接编译支持, 可以借助工具如: [llvm ir utils](https://github.com/compor/llvm-ir-cmake-utils/blob/master/cmake/LLVMIRUtil.cmake)

独立 LLVM Pass:

```
<project dir>/
    |
    CMakeLists.txt
    <pass name>/
        |
        CMakeLists.txt
        Pass.cpp
        ...
```

`<project>/CMakeLists.txt` 如下:
```cmake
find_package(LLVM REQUIRED CONFIG)

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
include_directories(${LLVM_INCLUDE_DIRS})

add_subdirectory(<pass name>)
```

`<pass>/CMakeLists.txt` 如下:
```cmake
add_library(LLVMPassname MODULE Pass.cpp)
```

如果想将独立 pass 转移至 llvm source tree, 修改如下:
```cmake
# <project>/CMakeLists.txt
list(APPEND CMAKE_MODULE_PATH "$PLLVM_CMAKE_DIR}")$
include(AddLLVM)

# <pass>/CMakeLists.txt 
add_llvm_library(LLVMPasname MODULE
	Pass.cpp
)
```

然后将 pass 文件夹拷贝到 `<LLVM root>/lib/Transforms` 文件夹, 然后在 `<LLVM root>/lib/Transforms/CMakeLists.txt` 下添加 `subdirectory(<pass name>)`

> 详见 [llvm CMake](https://llvm.org/docs/CMake.html)