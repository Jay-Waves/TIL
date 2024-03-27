clang -S .ll

opt -xload clang -xload mypass.so

`clang -xload mypass.so`


### Pass in Src Tree

```sh
cmake ../llvm -G "Unix Makefiles" \
	-DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;compiler-rt" \
	-DCMAKE_BUILD_TYPE=Release \
	-DLLVM_TARGETS_TO_BUILD="X86"
```

`make -j4`