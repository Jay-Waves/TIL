## 安装

Humble 版本只在 Ubuntu 22.04 上有 apt 安装方式, 其他版本需要自己编译源码.

**问题汇总:**

[rosdep debug](https://zhuanlan.zhihu.com/p/128035718)

### 编译 ros2-humble:

```bash
# 普通编译:
colcon build --symlink-install

# 修改:
colcon build --parallel-workers 4 # 并行任务上限
export MAKEFLAGS="-j1" # 指定编译线程数为1
colcon build --packages-select <pkg1> <pkg2> # 选特定包编译
```

在WSL2平台下, 普通编译ROS2无法通过. 参加[WSL配置](../../../../System/Distributions/WSL/配置%20-%20从此开始.md), 经尝试, 无论是否开启虚拟内存, 内存上限开多大 (本电脑最高16GB), 都无法通过编译. 现象为, 内存爆满卡死, 虚拟内存开启于事无补. 

> 越发认同: 虚拟内存是伪需求, 当内存爆满需要虚拟内存挽救时, 系统颠簸就已很大了.

[官方文档](https://docs.ros.org/en/humble/How-To-Guides/Installation-Troubleshooting.html#linux-troubleshooting) 描述内存受限设备, 可以降低线程数环境变量 `MAKEFLAGS`, 来减少内存使用.

在WSL2上, 配置`MAKEFLAGS`后, colcon仍同时编译多个模块, 造成系统资源爆满. 节省 colcon 编译有两个维度:
- 控制并行编译的包个数: `colcon build --parallel-workers 1`. 
- 控制每个包中 make 的并行任务数: `export MAKEFLAGS="-j4"`

编译过于缓慢, 很可能是并行数过多, 导致内存不够系统卡死. 不是等待更长时间就能解决的, 一次编译一个模块, 只需要1h就可以编译完成. 之前编译了7h还被宿主机杀了.

### 编译 nav2:

本机环境: ros2-humble, Ubuntu20.04, 宿主机 Windows10, 容器 WSL2. rosdep无法安装完整依赖, 部分依赖需要手动安装, 见[issue #3062](https://github.com/ros-planning/navigation2/issues/3062), 问题是:

>Rosdep will not work on out of REP2000 operating systems. You will need to build all of those dependencies yourself if you want to use Humble on 20.04, or use docker as Adam suggests if you don’t have to run on bare metal.
>
> You can find a list of the dependencies in tools/underlay.repos. That contains our non-default ROS 2 packages required to build Nav2

缺少包如下:
```bash
git clone -b master https://github.com/BehaviorTree/BehaviorTree.CPP.git
git clone -b ros2 https://github.com/ros/angles.git
git clone -b ros2 https://github.com/ros-simulation/gazebo_ros_pkgs.git
git clone -b ros2 https://github.com/ros-perception/vision_opencv.git
git clone -b ros2 https://github.com/ros/bond_core.git
git clone -b main https://github.com/ompl/ompl.git
git clone -b ros2 https://github.com/ros-simulation/gazebo_ros_pkgs.git
git clone -b rolling https://github.com/ros2/geometry2.git
```
