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

## [Issue#4175](https://github.com/ros-planning/navigation2/issues/4175), [PR#4180](https://github.com/ros-planning/navigation2/pull/4180), 

## [Issue#4166](https://github.com/ros-planning/navigation2/issues/4166), [Pull#4176](https://github.com/ros-planning/navigation2/pull/4176), [RclCpp Issue#2447](https://github.com/ros2/rclcpp/issues/2447)

我们和 ros2 底层设计人员讨论了 ros2 节点退出机制时可能出现的并发问题.

```cpp
nav2_util::CallbackReturn
AmclNode::on_cleanup(const rclcpp_lifecycle::State & /*state*/)
{
  executor_thread_.reset();

  // Get rid of the inputs first (services and message filter input), so we
  // don't continue to process incoming messages
  global_loc_srv_.reset();
  nomotion_update_srv_.reset();
  initial_pose_sub_.reset();
  laser_scan_connection_.disconnect();
  tf_listener_.reset();  //  listener may access lase_scan_filter_, so it should be reset earlier
  laser_scan_filter_.reset();
  laser_scan_sub_.reset();

  // Map
  map_sub_.reset();  //  map_sub_ may access map_, so it should be reset earlier
  if (map_ != NULL) {
    map_free(map_);
    map_ = nullptr;
  }
  first_map_received_ = false;
  free_space_indices.resize(0);

  // Transforms
  tf_broadcaster_.reset();
  tf_buffer_.reset();
```

```cpp
void
AmclNode::initTransforms()
{
  RCLCPP_INFO(get_logger(), "initTransforms");

  // Initialize transform listener and broadcaster
  tf_buffer_ = std::make_shared<tf2_ros::Buffer>(get_clock());
  auto timer_interface = std::make_shared<tf2_ros::CreateTimerROS>(
    get_node_base_interface(),
    get_node_timers_interface(),
    callback_group_);
  tf_buffer_->setCreateTimerInterface(timer_interface);
  tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
  tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(shared_from_this());

  sent_first_transform_ = false;
  latest_tf_valid_ = false;
  latest_tf_ = tf2::Transform::getIdentity();
}
```

1. `map_.reset()`
2. `map_server_.reset()`, due to shared_ptr's attribute, actually not reset.
3. still have `executor -> map_server_`
4. `map_server->map_` UAF!!
