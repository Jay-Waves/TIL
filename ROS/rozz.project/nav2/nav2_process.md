用 `kill -2 <pid>` 来杀掉进程, 是可以干净杀掉 nav2. 其启动的所有进程都可以顺利关闭 用 round_run 来控制.

- 用户输入 (pose) 都用 nav2_commander 写, 也用 launch 调用
- nav2_brinup_launch 也用 rozz_launch 调用
- 参数文件变异外部实现 (为什么不对?)