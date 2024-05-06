## bag

`ros bag` 工具用于记录订阅的消息

查看某订阅的消息: `ros2 topic echo <topic_name>`, 查看某订阅发布频率: `ros2 topic hz <topic_name>`

同时查看多个订阅信息, 并指定输出文件 `ros2 bag record -o <out_file_name> /turtle1/cmd_vel /turtle1/pose`

查看 bag 输出文件的详细信息: `ros2 bag info <out_file_name>`

将输出文件导入: `ros2 bag play <out_file_name>`

拿 bag 当时钟源: rosbag play --clock

## topic_tools

见 [topic_tools.git](https://github.com/ros-tooling/topic_tools)

```sh
# 订阅转发 relay 
ros2 run topic_tools relay /rtabmap/map /map

# 订阅 input topic, 并根据其内容 (用 m 代指) 转发到另一个 topic
ros2 run topic_tools relay_field <input topic> <output topic> <output type> [<expression on m>]

# 订阅 input topic, 并将其内容(字段) 按格式填入 output topic
ros2 run topic_tools transform <input topic> <output topic> <output type> [<expressoin on m>] [--import <modules>] [--field <topic_field>]

# 延迟
ros2 run topic_tools delay <topic_in> <delay/sec> [topic_out]

# 删除 X/Y 个消息, 并转发
ros2 run topic_tools drop <topic_in> <X> <Y> [topic_out]

# 减少消息转发速率
ros2 run topic_tools throttle messages <topic_in> <msgs_per_sec> [topic_out]

# 在多个输入topic中选择一个流
ros2 run topic_tools mux <topic_out> <topic_in1> [topic_in2 ...]
```

## interface

查看各种消息的接口格式

<pre>
$ ros2 interface show nav2_msgs/msg/Costmap

# This represents a 2-D grid map, in which each cell has an associated cost
std_msgs/Header header
        builtin_interfaces/Time stamp
                int32 sec
                uint32 nanosec
        string frame_id

# MetaData for the map
CostmapMetaData metadata
        builtin_interfaces/Time map_load_time
                int32 sec
                uint32 nanosec
        builtin_interfaces/Time update_time
                int32 sec
                uint32 nanosec
        string layer
        float32 resolution
        uint32 size_x
        uint32 size_y
</pre>