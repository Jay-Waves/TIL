AT&T 是类 Unix 系统的标准汇编语法. 其标准格式如下:

```
instru src dst
```

数据传输指令:

```asm
mov $0x05 %ax        ->  Reg[ax] = 0x05
mov %ax, -4(%bp)     ->  Mem[Reg[bp]-4] = Reg[ax]
mov -4(%bp), %ax     ->  Reg[ax] = Mem[Reg[bp]-4]
push $0x03           ->  Mem[Reg[sp]] = 0x03, Reg[sp] -= 4
pop                  ->  Reg[sp] += 4
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
subl $0x05, %eax     -> Reg[eax] = Reg[eax]-0x05
subl %eax, -4(%ebp)  -> Mem[Reg[ebp]-4] = Mem[Reg[ebp]-4]-Reg[eax]
```

跳转指令:

```asm
cmpl %eax %ebx       -> if Reg[eax]-Reg[ebx], set flags
jmp loc              -> jump to loc
je loc               -> if flags='eq', jump to loc
jg, jge, jl, gle, jnz...
```

栈与地址管理指令

```asm
pushl %eax           -> push Reg[eax], Reg[sp]-4
popl %eax            -> pop to Reg[eax]
leave                -> movl %ebp %esp; pop %ebp
lea 8(%esp) %esi     -> load effective addr, Reg[esi] = Reg[esp]+8
```

函数调用指令:

```asm
call label
ret
leave
```