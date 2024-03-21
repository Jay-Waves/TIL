## [Issue#4177](https://github.com/ros-planning/navigation2/issues/4177)

```
ros2 topic pub /map nav_msgs/msg/OccupancyGrid "
header:
  stamp:
    sec: 0
    nanosec: 0
  frame_id: map
info:
  map_load_time:
    sec: 0
    nanosec: 0
  resolution: 0.05000000074505806
  width: 2
  height: 2
  origin:
    position:
      x: -10.0
      y: -10.0
      z: 0.0
    orientation:
      x: 0.0
      y: 0.0
      z: 0.0
      w: 1.0
data: [-1, ] " <- bufferoverflow
```

size_x_ 和 size_y_ 如果大于数组 data 大小, 就会导致访问越界 (SEGV)

初始化时发生:

```cpp
  // create the costmap
  costmap_ = new unsigned char[size_x_ * size_y_];
  
  for (unsigned int it = 0; it < size_x_ * size_y_; it++) {
    data = map.data[it]; // <- SEGV
    if (data == nav2_util::OCC_GRID_UNKNOWN) {
      costmap_[it] = NO_INFORMATION;
    } else {
		...
    }
  }
```

过程中, 接收到 `/map` 消息触发 (危险!!!)

```cpp
void CostmapSubscriber::toCostmap2D()
{
  auto current_costmap_msg = std::atomic_load(&costmap_msg_);
	...

  unsigned char * master_array = costmap_->getCharMap();
  unsigned int index = 0;
  for (unsigned int i = 0; i < current_costmap_msg->metadata.size_x; ++i) {
    for (unsigned int j = 0; j < current_costmap_msg->metadata.size_y; ++j) {
      master_array[index] = current_costmap_msg->data[index]; //<-SEGV
      ++index;
    }
  }
}
```

## [PR#3972](https://github.com/ros-planning/navigation2/pull/3958) [PR#3958](https://github.com/ros-planning/navigation2/pull/3958) [Issue#3940](https://github.com/ros-planning/navigation2/issues/3940)

历经两个 PullRequest 才完美解决的并发 bug, 并且节省了很多不必要的检查.

Controller 和 Planner 节点在子线程中运行 CostmapROS 节点, 但是它们三个都是 ROS Lifecycle Node 的子类, 该类用于管理 ROS2 节点的生命周期启动和关闭, 在程序结束时会同时捕捉系统的停止信号, 此时它们三个的关闭顺序是不确定的. 

如果 CostmapROS 先关闭释放资源, Planner 和 Controller 仍会访问他们的 CostmapROS 子线程, 导致 UAF.

```cpp
// Planner or Controller:
Controller::on_cleanup(const rclcpp_lifecycle::State &){
	costmap_ros_->cleanup() // <-UAF, costmap already cleanup itself.
}
```

