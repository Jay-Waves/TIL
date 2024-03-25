---
url: https://github.com/GoesM/ros2_msg_interceptor/issues/7
tags:
  - Issue
---

### 最终解决方案

不开 coverage, 不开 asan, 使用 clang 编译 navigation2

clang 检查比较严格, 可能报 gcc 不报的错. 参考 [使用 clang 编译](nav2%20compile.md#使用%20clang%20编译)

```shell
source /opt/ros/humble/setup.bash
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++

	rosdep install -y --from-paths ./src --ignore-src

# 设置 make 并行数
export MAKEFLAGS="-j4"

colcon build \
	--symlink-install \
	--parallel-workers 2 \
	--cmake-args \
		-DBUILD_TESTING=OFF \
		-DCMAKE_CXX_STANDARD=17 \
		-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
		-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} \
			-w -Wno-error -Wno-format-securtiy" \
		-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS}  \
			-w -Wno-error -Wno-format-security"
```

由于 colcon 是增量编译, 所以我们只给特定包开启 asan 和 coverage 即可:

```shell
colcon build \
	--symlink-install \
	--cmake-clean-cache \
	--packages-select <pkg_name> \
	--cmake-args \
		-DBUILD_TESTING=OFF \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_CXX_STANDARD=17 \
		-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
		-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} \
			-w -Wno-error -Wno-everything \
			-fsanitize=address \
			--coverage -DCOVERAGE_RUN=1"  \
		-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS} \
			-w -Wno-error -Wno-format-security \
			-fsanitize=address \
			--coverage -DCOVERAGE_RUN=1" 
```


**编译后运行情况**: (关 composition)
```
pkg_name                   asan     new_delete_type_mismatch
amcl                       y        y
nav2_bt_navigator          n        n
nav2_behaviors             n        n
```

使用 sancov 覆盖率编译:

```shell
source /opt/ros/humble/setup.bash
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++

colcon build \
	--symlink-install \
	--cmake-clean-cache \
	--packages-select <pkg_name> \
	--cmake-args \
		-DBUILD_TESTING=OFF \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} \
			-w -Wno-error -Wno-everything \
			-fsanitize=address,thread \
			-fsanitize-coverage=edge,trace-pc-guard \
			--coverage -DCOVERAGE_RUN=1"  \
		-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS} \
			-w -Wno-error -Wno-format-security \
			-fsanitize=address,thread \
			-fsanitize-coverage=edge,trace-pc-guard \
			--coverage -DCOVERAGE_RUN=1" 
```

> [sancov](https://clang.llvm.org/docs/SanitizerCoverage.html)

## debug 记录

### 针对 nav2_amcl 的 fuzz

**amcl**: 自适应蒙特卡洛位置计算节点, 接受 laser 的订阅消息. 记录在 amcl_node.cpp:1505:initPubSub 的 topic 如下:
- `particle_cloud`: 粒子滤波器的状态信息, 消息类型是 nav2_msgs/msg/ParticleCloud.
- `amcl_pose`: 机器人位姿信息, 消息类型是 geometry_msgs/msg/PoseWithCovarianceStamped.

修改 topic 名字, 接到我们的中间节点上, 实现消息拦截. 

我觉得 fuzz amcl_pose 这个订阅比较好, 是 ros 内置节点. 不过有如下问题:
1. 位置信息乱窜会出 bug 吗?
2. amcl_pose 发布的消息, 会附加上 laser_scan.stamp (即激光传感器的时间戳) 进行同步. 修改消息流好像没有效果. 不如直接fuzz laser_scan 的传感信息 (nav2_bringup 包负责).

### 编译设置

以 nav2_amcl 为例:

在 bash 中设置编译器为 clang:
```bash
export CXX=clang++
```

在 nav2_amcl/Cmakelist 中添加:
```cmake
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage --fsanitize=all --verbose")
# --verbose 输出编译详细信息, 方便 debug
# --fsanitize=all 开启全部 sanitizer, 会显著增加编译时间
# --coverage 开启代码覆盖.
```

编译:
```bash
colcon build --packages-up-to nav2_util --cmake-clean-cache
```

如果编译时间太长没有成功, 可以查看 ws/log 中的日志信息, colcon 默认不会输出. 

在 Ubuntu22.04 x64, ros humble 上编译通过.

**!!! 运行会出现错误, 带 san 的 obj 文件无法和不带 san 的 obj 文件链接. 尝试整体编译:**

```bash
# 检查依赖
rosdep install -y --from-paths ./src --ignore-src

# 设置 make 并行数
export MAKEFLAGS="-j4"

colcon build \
	--symlink-install \
	--parallel-workers 1 \
	--cmake-clean-cache \
	--cmake-args \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_TESTING=OFF \
		-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} -fsanitize=address" \
		-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS} -fsanitize=address"
```

`--parallel-workers 1` 控制 colcon 一次编译一个包, 但仍会并行 make (默认核心数个 make 线程). 需要额外指定 make 参数 `-j4`.


### 运行 nav2

```bash
source /opt/ros/humble/setup.bash
source ./install/setup.bash
export TURTLEBOT3_MODEL=waffle
export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:/opt/ros/humble/share/turtlebot3_gazebo/models

# export ASAN_OPTIONS="log_path=~/.ros/asan.log new-delete-type-mismatch=0"

export ASAN_OPTIONS="new_delete_type_mismatch=0 detect_leaks=0"

export ASAN_OPTIONS="new_delete_type_mismatch=1 detect_leaks=1 halt_on_error=0"

export ASAN_OPTIONS=log_path=/path/to/...

LD_PRELOAD="/usr/lib/x86_64-linux-gnu/libasan.so.6 /usr/lib/x86_64-linux-gnu/libstdc++.so.6" \
ros2 launch nav2_bringup tb3_simulation_launch.py headless:=False \
2>&1 | grep -E '^\[INFO\] |^\[ERROR\]' 

```

ASAN 在 **[WSL](../../../Operating%20System/Linux%20Distributions/Ubuntu.md)** 上总是无法正常运行, 找到 [issue121](https://github.com/microsoft/WSL/issues/121)中描述:

> Like I said few months ago, ASAN is affected by [#1671](https://github.com/microsoft/WSL/issues/1671) like many other tools that maps large amount of memory. Until that's fixed I doubt it will be usable.

一般权限无法写入, 可以使用如下写入方式
```bash
bash -c " echo 1 > /proc/sys/vm/overcommit_memory" # 修复 Addrsan

bash -c " echo 0 > /proc/sys/kernel/yama/ptrace_scope" # 修复 LeakSan
```

https://github.com/google/sanitizers/issues/708

https://stackoverflow.com/questions/73924921/asan-not-flagging-leaks-on-wsl-2

### 带 ASan 编译单个包

修改 CMakeList.txt: 为了单个带 asan 包在整个不带 asan 能正常运行, ASan 不能静态编译入程序 (即编译时开启 -fsanitize=address), 必须动态链接到 libsan.so 来运行 (添加 -shared-libasan).

```cmake
# after the 'include_directories' section
set(ASAN_COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer -shared-libasan")
set(ASAN_LINK_FLAGS "-fsanitize=address")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ASAN_COMPILE_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ASAN_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ASAN_LINK_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${ASAN_LINK_FLAGS}")
```

build: `colcon build --packages-select nav2_amcl`

run: `export LD_PRELOAD=/path/to/libasan.so ros2 run nav2_amcl amcl`

LD_PRELOAD 会影响所有后续程序, 是否可以只影响其中一部分呢? 比如只对单个被sanitizer编译的包开启libasan.so, 其他不变?


```makefile
.PHONY: using-gcc using-gcc-static using-clang

using-gcc:
	g++-4.8 -o main-gcc -lasan -O -g -fsanitize=address -fno-omit-frame-pointer main.cpp && \
	ASAN_OPTIONS=symbolize=1 ASAN_SYMBOLIZER_PATH=$(shell which llvm-symbolizer) ./main-gcc

using-gcc-static:
	g++-4.8 -o main-gcc-static -static-libstdc++ -static-libasan -O -g -fsanitize=address -fno-omit-frame-pointer main.cpp && \
	ASAN_OPTIONS=symbolize=1 ASAN_SYMBOLIZER_PATH=$(shell which llvm-symbolizer) ./main-gcc-static

using-clang:
	clang -o main-clang -x c++ -O -g -fsanitize=address main.cpp && \
	ASAN_OPTIONS=symbolize=1 ASAN_SYMBOLIZER_PATH=$(shell which llvm-symbolizer) ./main-clang
```

> https://stackoverflow.com/questions/47021422/how-to-enable-address-sanitizer-for-multiple-c-binaries

> https://github.com/google/sanitizers/wiki/AddressSanitizerAsDso

> https://stackoverflow.com/questions/61967141/can-i-apply-c-sanitizer-to-only-my-part-of-the-program-but-not-thirdparty-libr?rq=3

> https://github.com/google/sanitizers/issues/89

==已解决==, 见 [nav2 bringup](nav2%20bringup.md)

### 使用 clang 编译

libstdc++ 失败

```bash
# 使用 clang
colcon build \
	--symlink-install \
	--parallel-workers 1 \
	--cmake-clean-cache \
	--cmake-args \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_TESTING=OFF \
		-DCMAKE_CXX_STANDARD=17 \ 
		-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} -w" \
		-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS} -w" \
		-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++

# -stdlib=libstdc++, libc++ fror llvm
# -frelaxed-template-template-args
#  DCMAKE_CXX_STANDARD=17
```

```bash
colcon build \
	--symlink-install \
	--parallel-workers 1 \
	--cmake-clean-cache \
	--packages-up-to nav2_costmap_2d \
	--cmake-args \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_TESTING=OFF \
		-DCMAKE_CXX_STANDARD=17 \
		-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} --verbose" \
		-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS}"

colcon build \
	--symlink-install \
	--parallel-workers 1 \
	--cmake-clean-cache \
	--cmake-args \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_TESTING=OFF \
		-DCMAKE_CXX_STANDARD=17 \
		-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} -w --verbose" \
		-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS} -w" \
		-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
# fail in nav2_costmap at 23/09/23 00:24


colcon build \
	--symlink-install \
	--parallel-workers 1 \
	--packages-select nav2_msgs \
	--cmake-clean-cache \
	--cmake-args \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_TESTING=OFF \
		-DCMAKE_CXX_STANDARD=17 \
		-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
		-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} --verbose \
			-w -Wno-error -Wno-format-security \
			-nostdlib \
			-I/usr/include/c++/9/ \
			-I/usr/include/x86_64-linux-gnu/c++/9/ \
			-I/usr/lib/gcc/x86_64-linux-gnu/9/include \
			-L/usr/lib/gcc/x86_64-linux-gnu/9" \
		-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS}  \
			-Wno-error -w -Wno-format-security"

# -nostdinc 
# /usr/include/c++/9/                      c++ std libs
# /usr/include/x86_64-linux-gnu/c++/9/     architecture-specific c++ std libs
# /usr/lib/gcc/x86_64-linux-gnu/9/include  gcc specific includes
```
	

```bash
clang++ test.cpp -o test \
-v \
--gcc-toolchain=/usr/lib/gcc/x86_64-linux-gnu/9 \
-I/usr/include/c++/9/ \
-I/usr/include/x86_64-linux-gnu/c++/9/ \
-I/usr/lib/gcc/x86_64-linux-gnu/9/include \
-L/usr/lib/gcc/x86_64-linux-gnu/9/ \
-L/usr/lib/x86_64-linux-gnu/9/ \
-B/usr/lib/gcc/x86_64-linux-gnu/9/


colcon build \
	--symlink-install \
	--parallel-workers 1 \
	--packages-up-to nav2_costmap_2d \
	--cmake-clean-cache \
	--cmake-args \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_TESTING=OFF \
		-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
		-DCMAKE_CXX_STANDARD=17 \
		-DCMAKE_EXE_LINKER_FLAGS="${CMAKE_EXE_LINKER_FLAGS} \
			-L/usr/lib/gcc/x86_64-linux-gnu/9 \
			-B/usr/lib/gcc/x86_64-linux-gnu/9" \
		-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} -v\
			-w -Wno-error -Wno-format-security \
			--gcc-toolchain=/usr/lib/gcc/x86_64-linux-gnu/9 \
			-I/usr/include/c++/9 \
			-I/usr/include/x86_64-linux-gnu/c++/9 \
			-I/usr/lib/gcc/x86_64-linux-gnu/9/include \
			-L/usr/lib/gcc/x86_64-linux-gnu/9 \
			-B/usr/lib/gcc/x86_64-linux-gnu/9" \
		-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS}  \
			-Wno-error -w -Wno-format-security \
			--gcc-toolchain=/usr/lib/gcc/x86_64-linux-gnu/9"

colcon build \
	--symlink-install \
	--parallel-workers 1 \
	--cmake-clean-cache \
	--cmake-args \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_TESTING=OFF \
		-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
		-DCMAKE_CXX_STANDARD=17 \
		-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} -v\
			-w -Wno-error -Wno-everything" \
		-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS}  \
			-w -Wno-error -Wno-format-security"

colcon build \
	--symlink-install \
	--parallel-workers 1 \
	--cmake-args \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_TESTING=OFF \
		-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
		-DCMAKE_CXX_STANDARD=17 \
		-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} \
			-w -Wno-error" \
		-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS}  \
			-w -Wno-error"
```

==编译错误:==

nav2_costmap_2d 中 image.hpp 和 image_process.hpp 出现编译错误, 使用 stl_pair.h 时参数应该是 const

nav2_mppi_controller 把 CMakeLists.txt 中 `target_compile_options(${lib} PUBLIC  -fconcepts)` 中 `fconcepts` 删掉.

**我们已经在[PR#3900](https://github.com/ros-planning/navigation2/pull/3900)提交了这个问题, 现在用clang编译不会出错.**

### 修改 nav2_bringup 运行单个包

经常爆出内存问题 (如 new-delete), 可能是没用 Asan 编译的代码和用了 Asan 编译的代码进行了内存交互, 这导致了 Asan 错误地将这些代码. amcl 带 Asan 报错的堆栈: 可以看到是 nav2 调用的底层声明节点的时候出错了.

```
    #0 0x7f337227624f in operator delete(void*, unsigned long) ../../../../src/libsanitizer/asan/asan_new_delete.cpp:172
    #1 0x7f3371e1ea4e in __gnu_cxx::new_allocator<char>::deallocate(char*, unsigned long) /usr/include/c++/11/ext/new_allocator.h:145
    #2 0x7f3371e17573 in std::allocator_traits<std::allocator<char> >::deallocate(std::allocator<char>&, char*, unsigned long) /usr/include/c++/11/bits/alloc_traits.h:496
    #3 0x7f3371e6c3a2 in void rclcpp::allocator::retyped_deallocate<char, std::allocator<char> >(void*, void*) /opt/ros/humble/include/rclcpp/rclcpp/allocator/allocator_common.hpp:67
    #4 0x7f337108cdae in rcutils_string_map_fini (/opt/ros/humble/lib/librcutils.so+0x9dae)
    #5 0x7f336f7f5cf5  (/opt/ros/humble/lib/librcl.so+0x22cf5)
    #6 0x7f336f7f6017 in rcl_node_resolve_name (/opt/ros/humble/lib/librcl.so+0x23017)
    #7 0x7f336f7f61e8 in rcl_publisher_init (/opt/ros/humble/lib/librcl.so+0x231e8)
    #8 0x7f3370fb367d in rclcpp::PublisherBase::PublisherBase(rclcpp::node_interfaces::NodeBaseInterface*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, rosidl_message_type_support_t const&, rcl_publisher_options_s const&) (/opt/ros/humble/lib/librclcpp.so+0x15767d)
    #9 0x7f3370f77ffb  (/opt/ros/humble/lib/librclcpp.so+0x11bffb)
    ...
```

实验验证: 用开了 san 编译的 ros 跑一跑只用 san 编译了的 amcl, 看看堆栈是否类似.

解决办法:
```bash
source /opt/ros/humble/setup.bash
source ./install/setup.bash

export ASAN_OPTIONS="new_delete_type_mismatch=1 detect_leaks=1 halt_on_error=0"

LD_PRELOAD="/usr/lib/x86_64-linux-gnu/libasan.so.6 /usr/lib/x86_64-linux-gnu/libstdc++.so.6" \
./amcl \
2>&1 | grep -E '^\[INFO\] |^\[ERROR\]' 
```

编译顺序:
```
Starting >>> nav2_common
Starting >>> nav2_msgs
Starting >>> nav2_util
Starting >>> nav2_voxel_grid
Starting >>> nav_2d_msgs
Starting >>> nav_2d_utils
Starting >>> dwb_msgs
Starting >>> nav2_behavior_tree
Starting >>> nav2_lifecycle_manager
Starting >>> nav2_map_server
Starting >>> nav2_costmap_2d
Starting >>> nav2_core
Starting >>> dwb_core
Starting >>> costmap_queue
Starting >>> nav2_controller
Starting >>> nav2_regulated_pure_pursuit_controller
Starting >>> dwb_critics
Starting >>> dwb_plugins
Starting >>> nav2_simple_commander
Starting >>> nav2_amcl  // amcl 被我改 Cmake 了, 卧槽
Starting >>> nav2_velocity_smoother
Starting >>> nav2_rviz_plugins // 这个应该不能带 asan, 保证 rviz 正常启动
Starting >>> nav2_collision_monitor
Starting >>> nav2_behaviors
Starting >>> nav2_bt_navigator
Starting >>> nav2_constrained_smoother
Starting >>> nav2_mppi_controller
Starting >>> nav2_navfn_planner
Starting >>> nav2_planner
Starting >>> nav2_smac_planner
Starting >>> nav2_smoother
Starting >>> nav2_theta_star_planner
Starting >>> nav2_waypoint_follower
Starting >>> nav2_rotation_shim_controller
Starting >>> nav2_dwb_controller
Starting >>> navigation2
Starting >>> nav2_bringup
Starting >>> nav2_system_tests
```