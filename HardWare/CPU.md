## CPU架构

![](../attach/Pasted%20image%2020240516211902.png)

- ALU, Arithmetic Logic Unit, 算术逻辑单元
- FPU, Floating Point Unit, 浮点运算单元
- Registers, 寄存器, 存储临时数据和指令
- Pipeline, 指令流水线
- Cache, 缓存
	- L1 Cache, 一级缓存, 离核心最近. 分为指令缓存 (I-Cache) 和数据缓存 (D-Cache)
	- L2 Cache, 二级缓存, 单核心独有
	- L3 Cache, 三级缓存, 多核心共享
- Control Unit, 控制单元
	- PC, Program Counter, 指令计数器
	- IR, Instruction Register, 指令寄存器
	- ID, Instruction Decoder, 指令解码器
- MMU, Memory Management Unit, 地址转换和内存保护.

## CPU速度

用户时间成本分为:
- 系统响应时间 -> 系统性能*system performance*
- cpu执行时间 -> CPU性能*cpu performance*

程序有多条指令构成, 当指令系统不变时, 一个程序执行的总指令数相对不变. CPU 使用时钟来协调计算运行, 该时钟非墙上时钟, 而是有固定频率, 称为**时钟频率** (如 3.6GHz), 时钟频率的倒数称为**时钟周期, clock cycle**, 时钟周期的时间间隔称为**时钟周期长度**.  

由于微指令存在, 一条指令不一定对应一个 CPU Clock, 所以引入**每条指令所耗平均时钟周期**, CPI, clock cycle per instruction, 即执行它将持续多少个时钟周期. 程序真实持续时间为: `指令平均时钟周期数 * 时钟周期长度 * 程序指令数`
