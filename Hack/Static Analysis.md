利用已有规则匹配源码中可能存在的漏洞.
- 对于脚本语言, 漏洞大部分来源于**没有过滤用户输入**, 使得输入传递到了敏感函数中执行 (如 SQL 注入, 反序列化, 远程命令执行); 其他漏洞则来源于**语言特性**.
- 对于编译语言, 除上述外, 还有**内存区域出现异常**, 不易被静态分析发现.

基本步骤: 

1. **匹配漏洞**.

2. **数据流跟踪**: 根据敏感函数的*函数调用图*, 对敏感参数进行溯源分析 (从下往上); 或判断输入能影响哪些变量和参数  (从上往下). 可以利用 ast 树结构进行分析, graphiz 绘图. 数据流分析需要大量递归和循环, 可能遇到性能问题.

3. **控制流分析**: 分析代码块的执行流程图, 并尝试对判断条件自动求解.

4. **函数交叉引用**.