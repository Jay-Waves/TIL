内存管理见[进程与线程](../../../System/Linux%20Basic/进程与线程.md)

栈使用的CPU寄存器:
- `EBP` Extended Base Pointer, 堆栈帧指针. 初始指向上一个函数堆栈帧基址.
- `ESP` Extended Stack Pointer, 堆栈顶指针. 初始指向函数堆栈帧底部.
- `EIP` Extended Instruction Pointer, 指向下次执行的指令

函数调用时: function(a, b, c), 
1. 压入参数: 从右开始压入, 使得a离ebp(第一个可知位置)最近.
```
push c
push b
push a
```
2. 压入返回地址: call会自动将下条指令的地址作为返回地址(RA)压入.
```
call function
```
3. 初始化栈 (此时已进入被调用函数的汇编)
```
push ebp
mov ebp esp
```
4. 初始化局部变量空间, 编译时其所用局部变量空间就已被固定. (静态空间)
```assembly
sub $0x32 %esp
```

栈从高地址向下发展:
```
...low  <-- ESP
[   ... local variables ...   ]  <-- ESP
[   saved EBP value   ]
[   return address    ]          <-- EBP
[   parameter a       ]
[   parameter b       ]
[   parameter c       ]
...high
```

隐式更新 %esp 的指令:

`push`   
1. `sub $4, %esp`      ; 减少堆栈指针，为32位值留出空间 
2. `mov %eax, (%esp)`  ; 将%eax的内容写入新的%esp指向的位置

`pop`:    
1. `mov (%esp), %ebx`  ; 从%esp当前指向的位置加载值到%ebx
2. `add $4, %esp`      ; 增加堆栈指针

`call`:
1. `push return address` ; 将返回地址压入堆栈
2. `jmp function_label`  ; 跳转到 function_label

`ret`:
1. `pop eip`     ; 从堆栈中取出返回地址到eip (指令指针)