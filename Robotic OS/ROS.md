新窗口, 首先执行 ROS 的配置文件.

```
source /opt/ros/humble/setup.bash
```

从包内运行某程序:

```
ros2 run <pkg_name> <exe_name>
```

## node

列出所有包名: `ros2 node list`

使用 **remap** 修改节点信息. 例如, 新开一个 turtlesim 节点, 命名为 my_turtle.

```
ros2 run turtlesim turtlesim_node --ros-args --remap __node:=my_turtle
```

查看节点详细信息: `ros2 node info <node_name>`

## topic

deliver messages between ros nodes, **many to many**.

可视化订阅: `rqt_graph` .

列出所有订阅: `ros2 topic list -t`, t 参数列出订阅类型. 比如查出: `/turtle1/cmd_vel [geometry/msg/Twist]`, 指 `cmd_vel` 订阅接收的消息类型为 `geometry` 库的 `/msg/Twist` 类型. 可以使用 `ros2 interface show geometry/msg/Twist` 查看其接口 (消息类型的数据格式).

查看订阅内容: `ros2 topic echo <topic_name>`

查看订阅详细信息: `ros2 topic info <topic_name>`

### topic publish

向订阅发布消息: `ros2 topic pub <topic_name> <msg_type> '<args>'`. 

`'<args>'` 指实际传递的值, 必须以 YAML 格式输入, 例子如:

```bash
ros2 topic pub --once /turtle1/cmd_vel geometry_msgs/msg/Twist "{linear: {x: 2.0, y: 0.0, z: 0.0}, angular: {x: 0.0, y: 0.0, z: 1.8}}"
```

`--once` 指发布一个消息然后离开退出; 还有 `--rate 1`, 指持续以 1Hz 频率发布消息.

## service

service 是某**个** node 作为 server 提供的, 其他节点作为 client 请求服务.

列出服务名: `ros2 service list`

列出服务类型: `ros2 service type <service_name>`, service 有收发两种类型. `ros2 service list -t  --show-types` 同时列出所有服务的种类.

根据服务类型找服务: `ros2 service find std_srvs/srv/Empty`

查看服务的接口: `ros2 interface show turtlesim/srv/Spawn`

调用服务: `ros2 service call <service_name> <service_type> <arguments>`, 例如 `ros2 service call /spawn turtlesim/srv/Spawn "{x: 2, y: 2, theta: 0.2, name: ''}"` 注意输入的参数是 YAML 格式的.

## parameter

获取参数类型: `ros2 param get <node_name> <parameter_name>`

设置参数值: `ros2 param set <node_name> <parameter_name> <value>`

获取某节点的所有参数值: `ros2 param dump <node_name>`

导入参数值: `ros2 param load <node_name> <parameter_file>`. 某些只读参数只有启动时可以更改, 使用: `ros2 run <package_name> <executable_name> --ros-args --params-file <file_name>`. 

*注意参数文件格式是 YAML*

## action

适合耗时任务, 提供稳定反馈. 使用 server-client 模式, 但可以中断. 由 *goal, feedback, result* 组成, 其中 goal 和 result 都是一个 service, 而 feedback 是一个 topic. 

`ros2 action list`

`ros2 action info <action_name>`

现实接口类型: `ros2 interface show <action_name>`

消息控制:

`ros2 action send_goal <action_name> <action_type> <values>` 其中 `<values>` 应使用 YAML格式数据.
