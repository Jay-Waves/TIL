## Print

`p [fmt] <expr>`

```gdb
p var1
p &var1    # 打印地址
p $rsp     # 打印寄存器地址
p $rsp+8   # 打印寄存器地址+8
p /x 23423 # 打印16进制
p *arr@3   # 打印 arr[3] 值
```

## Examine

`x /<n/fmt/u> <addr>` 直接查看内存值.

`n` 查看长度

`u` 请求字节数, 默认显示 `n*u` 字节值
- b, byte
- h, halfword
- w, word, 4 bytes
- g, giant word, 8 bytes

`fmt`: 显示格式
- d, decimal
- u, usigned decimal
- s, string
- c, char
- t, binary
- o, octal
- x, hex
- z, hex with zero padding
- f, float
- i, instruction
- a, addr
