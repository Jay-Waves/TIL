AT&T 是类 Unix 系统的标准汇编语法. 其标准格式如下:

```
instru src dst
```

数据传输指令:

```asm
mov $0x05 %ax        ->  R[ax]=0x05
mov %ax, -4(%bp)     ->  mem[R[bp]-4] = R[ax]
mov -4(%bp), %ax     ->  R[ax]=mem[R[bp]-4]
push $0x03           ->  mem[R[sp]] = 0x04, R[sp]-=4
pop                  ->  R[sp]+=4
call func
ret
```

指令后缀决定操作数位宽:
- `movb` -> 1byte
- `movw` -> 2byte
- `movl` -> 4byte
- `movq` -> 8byte

算术运算指令:

```
subl $0x05, %eax     -> R[eax]=R[eax]-0x05
subl %eax, -4(%ebp)  -> mem[R[ebp]-4]=mem[R[ebp]-4]-R[eax]
```

跳转指令:

```asm
cmpl %eax %ebx       -> if R[eax]-R[ebx], set flags
jmp loc              -> jump to loc
je loc               -> if flags='eq', jump to loc
jg, jge, jl, gle, jnz...
```

栈与地址管理指令

```asm
pushl %eax           -> push R[eax], R[sp]-4
popl %eax            -> pop to R[eax]
leave                -> movl %ebp %esp; pop %ebp
lea 8(%esp) %esi     -> load effective addr, R[esi]=R[esp]+8
```

函数调用指令:

```asm
call label
ret
leave
```