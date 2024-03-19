# ConVul

> [(ASE2019) CONVUL: An Effective Tool for Detecting Concurrency Vulnerabilities](../../../paper/CONVUL_An_Effective_Tool_for_Detecting_Concurrency_Vulnerabilities.pdf)

用 [Itel Pin Tool](https://www.intel.com/content/www/us/en/developer/articles/tool/pin-a-dynamic-binary-instrumentation-tool.html) 来二进制插桩. 具体而言, 插桩检测 **Memory Events**(`read, write, free`) 和 **Synchronozation Events**(`fork, join, unlock, wait..`).

用 Vector Clock 来理解并发模型中事件顺序, 见 [FastTrack, Efficient and Precise Dynamic Race Detection](FastTrack,%20Efficient%20and%20Precise%20Dynamic%20Race%20Detection.md). ConVul 定义: 必须有执行顺序的两事件的**距离** $D(e_{1}, e_{2})$ 为, 两事件之间 **加锁和释放锁(同线程), 获取和释放相关操作(不同线程)** 的次数. 如果两个事件的距离比较近, 或者有另外一个事件距离它们俩都很近, 那么这两个事件的执行顺序就可能被颠倒.

==感觉这篇有点简略了, ASE2019, 才4页. 也没公开源码==

针对 UAF, DF(doule-free), NullPtr 具体漏洞类型进行优化 (新增更多筛选条件, 如 DF 必须存在两个 `free` 和一个 `assign`)