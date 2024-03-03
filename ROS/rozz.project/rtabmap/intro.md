ros_humble 下运行 gazebo_turtle3_demo

```shell
# 启动 gazebo 模拟环境
export TURTLEBOT3_MODEL=waffle
ros2 launch turtlebot3_gazebo turtlebot3_world.launch.py

# 启动 rtabmap 和 rviz
ros2 launch rtabmap_launch rtabmap.launch.py \
   visual_odometry:=false \
   frame_id:=base_footprint \
   subscribe_scan:=true depth:=false \
   approx_sync:=true \
   odom_topic:=/odom \
   scan_topic:=/scan \
   qos:=2 \
   args:="-d --RGBD/NeighborLinkRefining true --Reg/Strategy 1" \
   use_sim_time:=true \
   rviz:=true

# 将 /rtabmap/map 转发到 /map, 让 rviz 接收到地图
ros2 run topic_tools relay /rtabmap/map /map

# 启动 navigation2, 让小车自主导航. 在 rviz 上发布目标即可.
ros2 launch nav2_bringup navigation_launch.py use_sim_time:=True

# 若因为 ASAN, rviz 启动失败, 可以手动启动rviz
ros2 launch nav2_bringup rviz_launch.py
```

## 编译

```shell
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
export MAKEFLAGS="-j4"

# 获取 ros 环境
source /opt/ros/humble/setup.zsh 

# 安装依赖
rosdep install -y --from-paths ./src --ignore-src

# 编译
colcon build \
	--symlink-install \
	--parallel-workers 2 \
	--cmake-args \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} \
			-w -Wno-error -Wno-everything \
			-fsanitize=address,undefined"  \
		-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS} \
			-w -Wno-error -Wno-format-security \
			-fsanitize=address,undefined"

# 将 rviz 相关包重新不开 asan 编译
colcon build \
	--symlink-install \
	--parallel-workers 2 \
	--packages-select rtabmap_rviz_plugins \
	--cmake-args \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} \
			-w -Wno-error -Wno-everything"  \
		-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS} \
			-w -Wno-error -Wno-format-security"

export ASAN_OPTIONS="new_delete_type_mismatch=0 detect_leaks=0 halt_on_erro=0"
```