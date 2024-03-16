---
date: 2023-09-01
tags:
  - Paper
---
> 参考 [Fuzzing: A Survey](../../../paper/Fuzzing%20a%20survey.pdf)

# Fuzz 综述

## Why Fuzz

#### 1 静态测试 

无需运行代码, 优点是检测速度快, 缺点是正确率低. 白家驹老师说静态测试较难.

#### 2 动态测试

需要运行代码, 优点是准确率高, 缺点如下:
- 需要测试人员高度参与, 低效, 对测试者能力要求高.
- 低覆盖率

#### 3 符号执行

Symbolic Execution, 指用符号值代替变量的数值, 对程序执行路径进行符号运算. 大规模的符号执行存在"路径复杂度爆炸"问题, 并且对"调用外部系统函数"这样的环境交互存在困难.

#### 4 模糊测试

Fuzz, 指对目标程序产生大量设计的输入值, 并检测是否有异常产生. 通常有以下四步骤:
1. testcase generation
2. testcase running
3. program execution state monitoring: Sanitizers
4. analysis of exceptions: locate the errors with **debuggers**

## Types

按构造 testcase 方式分:
- generation based fuzz: 需要**配置文件**来规定输入格式
- mutation based fuzz: 根据fuzz进程和**几个初始输入**产生更多testcase, 无需前置知识.

按监视程序 (monitoring of execution state) 是否对样例构造 (testcase generation) 有反馈, 分为:
- dumb fuzzer
- smart fuzzer: feedback be used to construct mutation of testcase

按对源码的依赖程度分:
- white box: know source code
- gray box: without source code, but have internal info
- black box:

按对程序的探索策略分, 核心问题是如何从执行路径中提取信息.
- directed fuzzer: 关注某些目标程序或特定数据类型
- coverage-based fuzzer: 尽可能覆盖所有程序及其分支  

常见fuzzer分类:  

|                  | White Box | Gray Box                                 | Black Box |
| ---------------- | --------- | ---------------------------------------- | --------- |
| Generation-based | SPIKE     |                                          |           |
| Mutation-based   | Miller    | AFL, Driller, Vuzzer, TaintScope, Mayhem | SAGE, Libfuzzer          |

## AFL

Coverage-based Fuzz, **take AFL for example**

coverage-based fuzzing 结构:

```python
T = Seed
Tx = Null
if T == Null:
	T.add(empty_file)

while not (timeout or abort_signal):
	t = choose_next(T)
	s = assign_energy(t)
	for i in range(1, s):
		t_i = mutate(t)
		if crashes(t_i):
			Tx.add(t_i)
		elif is_interesting(t_i):
			T.add(t_i)

print(Tx)
```

### Key Questions

- 1 get initial input: 尤其对于复杂输入, 初始种子需要格式良好.
- 2 generate testcases: 覆盖更多代码区域, 执行状态; 或提高效率.
- 3 select seed from pool: 根据代码覆盖率, 或者bug的特征
- 4 efficiently test application: 优化程序测试所消耗资源, 多线程运行等手段.

## Improvements Techniques

现代Fuzzer多是 mutation-based 的, 其有如下困难性:
- 难以裂变种子输入: 合理 mutate, 才能使样例覆盖率和bug触发率提高.
- 低代码覆盖率: 大部分样例只通过几个热点路径.
- 构造输入合法性低: 黑盒和灰盒构造大量非法样例, 浪费资源.

|                    | Testcase Generation |          | Program Execution |                  |
| ------------------ | ------------------- | -------- | ----------------- | ---------------- |
| Techniques         | Generation          | Mutation | Guiding           | Path exploration |
| Statics analysis   |                     | Y        | Y                 | Y                |
| Taint analysis     |                     | Y        | Y                 |                  |
| Instrumentation    |                     | Y        | Y                 | Y                |
| Symbolic execution |                     |          |                   | Y                |
| Machine learning   | Y                   | Y        |                   |                  |
| Formatt Method     | Y                    |          |                   |                  |

## Application

fuzz 针对以下几项:
- fuzz for 固定文件格式
- fuzz for 内核, 泛指本地程序 API 测试
- fuzz for 协议, 泛指网络测试
