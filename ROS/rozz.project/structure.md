- interceptor
- monitor
- bringup

```
fuzzer/src
│
├── fuzz.py   重复循环检测, 选择UserInput, 然后bringup
│
├── config.json: 原 params.py, fuzz参数
│
├── bringup/  启动单轮循环, 不决定UserInput
│   │  
│   ├── launch/
│   │   └── launch.py
│   │  
│   ├── params/
│   │   └── base_covr.json:  nav2 覆盖率信息
│   │  
│   └── plugins/
│       ├── shutdown/check clean  强制关闭ros2所有进程
│       └── 
│
├── interceptor/   信息变异
│   │  
│   ├── sensor messages/  变异消息
│   │   ├── odom_interceptor
│   │   └── scan_interceptor
│   │  
│   ├── user input/  变异用户输入
│   │   ├── goal pose
│   │   └── initial pose
│   │
│   └── configureatin parameters/  变异配置文件
│
├── monitor/                      报错整理, 信息收集
│   ├── bag record                用户输入记录
│   ├── bug analyzer              分析归类报错
│   ├── log controller            整理日志信息
│   └── code covearge reporter    代码覆盖检测
│
├── selftest  测试
│   
└── README.md
```


```
Planner Server
│
├─ cleanup(): lock(mutex)
│
├─ isCurrent(): wait_for_lock
│
├─ cleanup(): free layerd_costmap_, unlock
│
├─ isCurrent(): visit layered_costmap_->plugins_
│
├─ isCurrent(): Error: SEGV
```