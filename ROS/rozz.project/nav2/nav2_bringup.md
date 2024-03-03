---
date: 2023-09-26
tags:
  - Issue
url: https://github.com/GoesM/ros2_msg_interceptor/issues/9
---
launch 顺序:

`tb3_simulation_launch.py` -> `rviz_launch.py`
                       -> `bringup_launch.py`

`bringup_launch.py` -> nav2 container -> `slam_launch.py`
                                   -> `localiztion_launch.py`
                                   -> `navigation_launch.py`

`localiztion_launch.py` -> nav2_map_server node
                     -> nav2_amcl node
                     -> ( nav2_lifecycle_manager node)

`navigation_launch.py` -> nav2_controler node
					-> nav2_smoother node
					-> nav2_planner node
					-> nav2_behaviors node
					-> nav2_bt_navigator node
					-> nav2_waypoint_follower node
					-> nav2_velocity_smoother node
					-> nav2_lifecycle_manager node

```bash
./map_server 2>&1 | grep -E '^\[INFO\] |^\[ERROR\]' 
```

开启 asan 后, 不能使用 container. 也不必可以指定链接库 LD.
```bash
source /opt/ros/humble/setup.zsh
source ./install/setup.zsh # 工作区
export TURTLEBOT3_MODEL=waffle
export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:/opt/ros/humble/share/turtlebot3_gazebo/models
export ASAN_OPTIONS="new_delete_type_mismatch=0 detect_leaks=0 halt_on_erro=0"

#LD_PRELOAD="/usr/lib/x86_64-linux-gnu/libasan.so.6 /usr/lib/x86_64-linux-gnu/libstdc++.so.6"
ros2 launch nav2_bringup tb3_simulation_launch.py headless:=True use_composition:=False # log_level:=log
```

ros2 launch nav2_bringup tb3_simulation_launch.py headless:=False

相关日志:
```bash
export ROS_LOG_DIR=./runtime_log/
export ASAN_OPTIONS=log_path=./runtime_log/asan_log.%p
```

### 报错记录

- 标准 ros2 humble 环境
- 带 Asan 编译的 nav2 包

1. 不 preload asan.so
```
[INFO] [launch]: All log files can be found below /home/dirge/.ros/log/2023-09-25-11-20-23-486932-LAPTOP-J48L341S-3774
[INFO] [launch]: Default logging verbosity is set to INFO
[INFO] [gzserver-1]: process started with pid [3788]
[INFO] [gzclient-2]: process started with pid [3790]
[INFO] [spawn_entity.py-3]: process started with pid [3792]
[INFO] [robot_state_publisher-4]: process started with pid [3794]
[INFO] [rviz2-5]: process started with pid [3796]
[INFO] [component_container_isolated-6]: process started with pid [3798]
[ERROR] [component_container_isolated-6]: process has died [pid 3798, exit code 1, cmd '/opt/ros/humble/lib/rclcpp_components/component_container_isolated --ros-args --log-level info --ros-args -r __node:=nav2_container --params-file /tmp/tmp5oituuxp --params-file /tmp/launch_params_9czt3pi3 -r /tf:=tf -r /tf_static:=tf_static'].
[ERROR] [gzserver-1]: process has died [pid 3788, exit code 255, cmd 'gzserver -s libgazebo_ros_init.so -s libgazebo_ros_factory.so /home/dirge/code/ros_ws/install/nav2_bringup/share/nav2_bringup/worlds/world_only.model'].
[ERROR] [rviz2-5]: process has died [pid 3796, exit code 1, cmd '/opt/ros/humble/lib/rviz2/rviz2 -d /home/dirge/code/ros_ws/install/nav2_bringup/share/nav2_bringup/rviz/nav2_default_view.rviz --ros-args'].
[INFO] [robot_state_publisher-4]: sending signal 'SIGINT' to process[robot_state_publisher-4]
[INFO] [spawn_entity.py-3]: sending signal 'SIGINT' to process[spawn_entity.py-3]
[INFO] [gzclient-2]: sending signal 'SIGINT' to process[gzclient-2]
[ERROR] [spawn_entity.py-3]: process has died [pid 3792, exit code -2, cmd '/opt/ros/humble/lib/gazebo_ros/spawn_entity.py -entity turtlebot3_waffle -file /home/dirge/code/ros_ws/install/nav2_bringup/share/nav2_bringup/worlds/waffle.model -robot_namespace -x -2.00 -y -0.50 -z 0.01 -R 0.00 -P 0.00 -Y 0.00 --ros-args'].
[ERROR] [gzclient-2]: process has died [pid 3790, exit code -2, cmd 'gzclient'].
[INFO] [robot_state_publisher-4]: process has finished cleanly [pid 3794]
```

1. preload 仅给 container (以及其中很多 nodes) 开

修改 `launch.py`
```python
    # 设置 LD_PRELOAD 环境变量
    set_ld_preload_cmd = SetEnvironmentVariable(
        name='LD_PRELOAD',
        value='/usr/lib/x86_64-linux-gnu/libasan.so.6 /usr/lib/x86_64-linux-gnu/libstdc++.so.6'
    )
    # 添加设置 LD_PRELOAD 的命令
    ld.add_action(set_ld_preload_cmd)
```

```
[INFO] [launch]: All log files can be found below /home/dirge/.ros/log/2023-09-25-11-18-18-365394-LAPTOP-J48L341S-3579
[INFO] [launch]: Default logging verbosity is set to INFO
[INFO] [gzserver-1]: process started with pid [3593]
[INFO] [gzclient-2]: process started with pid [3595]
[INFO] [spawn_entity.py-3]: process started with pid [3597]
[INFO] [robot_state_publisher-4]: process started with pid [3599]
[INFO] [rviz2-5]: process started with pid [3601]
[INFO] [component_container_isolated-6]: process started with pid [3603]


[ERROR] [gzserver-1]: process has died [pid 3593, exit code 255, cmd 'gzserver -s libgazebo_ros_init.so -s libgazebo_ros_factory.so /home/dirge/code/ros_ws/install/nav2_bringup/share/nav2_bringup/worlds/world_only.model'].
[ERROR] [spawn_entity.py-3]: process has died [pid 3597, exit code 1, cmd '/opt/ros/humble/lib/gazebo_ros/spawn_entity.py -entity turtlebot3_waffle -file /home/dirge/code/ros_ws/install/nav2_bringup/share/nav2_bringup/worlds/waffle.model -robot_namespace -x -2.00 -y -0.50 -z 0.01 -R 0.00 -P 0.00 -Y 0.00 --ros-args'].
[ERROR] [rviz2-5]: process has died [pid 3601, exit code 1, cmd '/opt/ros/humble/lib/rviz2/rviz2 -d /home/dirge/code/ros_ws/install/nav2_bringup/share/nav2_bringup/rviz/nav2_default_view.rviz --ros-args'].

加载 container 正常了
[INFO] [launch_ros.actions.load_composable_nodes]: Loaded node '/controller_server' in container '/nav2_container'
[INFO] [launch_ros.actions.load_composable_nodes]: Loaded node '/map_server' in container '/nav2_container'
[INFO] [launch_ros.actions.load_composable_nodes]: Loaded node '/smoother_server' in container '/nav2_container'
[INFO] [launch_ros.actions.load_composable_nodes]: Loaded node '/amcl' in container '/nav2_container'

[INFO] [component_container_isolated-6]: sending signal 'SIGINT' to process[component_container_isolated-6]
[INFO] [robot_state_publisher-4]: sending signal 'SIGINT' to process[robot_state_publisher-4]

[INFO] [gzclient-2]: sending signal 'SIGINT' to process[gzclient-2]
[INFO] [robot_state_publisher-4]: process has finished cleanly [pid 3599]
[ERROR] [gzclient-2]: process has died [pid 3595, exit code -11, cmd 'gzclient'].

抽到 ctrl+c, 但是 asan 会阻止该 sigint 生效.
[ERROR] [component_container_isolated-6]: process[component_container_isolated-6] failed to terminate '5' seconds after receiving 'SIGINT', escalating to 'SIGTERM'
[INFO] [component_container_isolated-6]: sending signal 'SIGTERM' to process[component_container_isolated-6]
[ERROR] [component_container_isolated-6]: process[component_container_isolated-6] failed to terminate '10.0' seconds after receiving 'SIGTERM', escalating to 'SIGKILL'
!! 这句话意味着 asan 正常启动了, 用 sigterm 杀不掉, 必须用 sigkill
[INFO] [component_container_isolated-6]: sending signal 'SIGKILL' to process[component_container_isolated-6]
[ERROR] [component_container_isolated-6]: process has died [pid 3603, exit code -9, cmd '/opt/ros/humble/lib/rclcpp_components/component_container_isolated --ros-args --log-level info --ros-args -r __node:=nav2_container --params-file /tmp/tmpo75z4h4e --params-file /tmp/launch_params_rnenhp0o -r /tf:=tf -r /tf_static:=tf_static'].
```

**rviz**

注意到 `rviz` 命令 `/opt/ros/humble/lib/rviz2/rviz2 -d /home/dirge/code/ros_ws/install/nav2_bringup/share/nav2_bringup/rviz/nav2_default_view.rviz --ros-args` 报错 asan not preload.

```bash
export ASAN_OPTIONS="new_delete_type_mismatch=0 detect_leaks=0"
LD_PRELOAD="/usr/lib/x86_64-linux-gnu/libasan.so.6 /usr/lib/x86_64-linux-gnu/libstdc++.so.6" \
/opt/ros/humble/lib/rviz2/rviz2 -d /home/dirge/code/ros_ws/install/nav2_bringup/share/nav2_bringup/rviz/nav2_default_view.rviz --ros-args 

export ASAN_OPTIONS="new_delete_type_mismatch=1 detect_leaks=1 halt_on_error=0"
```

non-asan-ros, 
`/opt/ros/humble/lib/rviz2/rviz2 -d  /home/dirge/src/nav2_gcc/install/nav2_bringup/share/nav2_bringup/rviz/nav2_default_view.rviz --ros-args`

non-asan-ros, rviz with preload asan.so: 无法启动

asan-ros, asan-rviz with preload asan.so:
```bash
LD_PRELOAD="/usr/lib/x86_64-linux-gnu/libasan.so.6 /usr/lib/x86_64-linux-gnu/libstdc++.so.6" \
rviz2 \
2>&1 | grep -E '^\[INFO\] |^\[ERROR\]' 
```
启动, 但报错说无法加载某些plugin. 

==应避免 asan.so 影响到 rviz==

**container**

已正常启动. 



```bash
source /opt/ros/humble/setup.zsh
source ./install/setup.zsh # 工作区
export TURTLEBOT3_MODEL=waffle
export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:/opt/ros/humble/share/turtlebot3_gazebo/models
export ASAN_OPTIONS="new_delete_type_mismatch=1 detect_leaks=1 halt_on_error=0"

# LD_PRELOAD="/usr/lib/x86_64-linux-gnu/libasan.so.6 /usr/lib/x86_64-linux-gnu/libstdc++.so.6" \
ros2 launch nav2_bringup tb3_simulation_launch.py headless:=False \
2>&1 | grep -E '^\[INFO\] |^\[ERROR\] |^\[DEBUG\]'  
```

### 手动启动 nav2_container

声明一个空的 nav2_container 容器:
```bash
export ASAN_OPTIONS="new_delete_type_mismatch=1 detect_leaks=1 halt_on_error=0"

LD_PRELOAD="/usr/lib/x86_64-linux-gnu/libasan.so.6 /usr/lib/x86_64-linux-gnu/libstdc++.so.6" \
/opt/ros/humble/lib/rclcpp_components/component_container_isolated \
	--ros-args --log-level debug \
	--ros-args -r __node:=nav2_container \
	--params-file /tmp/tmp2ky3_oyv \
	--params-file /tmp/launch_params_rawncdfx \
	-r /tf:=tf -r /tf_static:=tf_static \
2>&1 | grep -E '^\[INFO\] |^\[ERROR\] |^\[DEBUG\]' 
```

查看容器名称: /nav2_container

`ros2 component lsit`

以 `localiztion_launch.py` 中挂载的节点为例:

```bash
ros2 component load /nav2_container nav2_map_server nav2_map_server::MapServer

ros2 component load /nav2_container nav2_amcl nav2_amcl::AmclNode

ros2 component load /nav2_container nav2_lifecycle_manager nav2_lifecycle_manager::LifecycleManager -p node_names:="['/map_server', '/amcl']"
```

成功加载.

### 手动启动 rviz

用没有开 Asan 的 nav2_rviz_plugins 启动 rviz2 还是能启动的.

```bash
 rviz2 -d  /home/dirge/src/nav2_gcc/install/nav2_bringup/share/nav2_bringup/rviz/nav2_default_view.rviz --ros-args
```

### 禁用 Container, 各节点独立启动

麻烦的办法是修改 bringup, 但用 `use_composition:=False` 最方便.

```bash
source /opt/ros/humble/setup.zsh
source ./install/setup.zsh # 工作区
export TURTLEBOT3_MODEL=waffle
export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:/opt/ros/humble/share/turtlebot3_gazebo/models
export ASAN_OPTIONS="new_delete_type_mismatch=1 detect_leaks=1 halt_on_error=0"
ros2 launch nav2_bringup tb3_simulation_launch.py \
	headless:=False use_composition:=False \
2>&1 | grep -E '^\[INFO\] |^\[ERROR\] |^\[DEBUG\]' 
```

此时==成功启动==. 

试着分析一下以前遇到的问题:

ros2 component 功能会将多个节点放在**同一个容器**中运行, 容器中组件会共享动态链接库 (.so文件).

如果使用 component 加载 amcl with asan, amcl 会试图向容器内添加一个 asan.so, 但是 component (在此语境下指 nav2_container) 会拒绝这一请求, 并终止 amcl 或 迫使 amcl 不带 asan.so 运行.

- 强行终止 amcl 时, 进程会卡死.
- 不带 asan.so 运行 amcl 时, asan 会报错需要用 LD_PRELOAD 提前启动一个 asan.so, 但其实 asan.so 本应被自动添加在 amcl 前, 只不过被 conponent 阻止了. 此时如果强行添加 ld_preload 在众多未开启 asan 的组件前, 同样会导致程序卡死.

事实上 amcl with asan 不需要 ld_preload. 添加后反而会导致错误, 可能是 amcl with asan 启动慢, use_sim_time 导致时间不同步了.