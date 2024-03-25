rclpy (ROS 2 Client Library for Python) 是一个用于Python编程语言的ROS 2 (Robot Operating System 2) 客户端库

### 基础类: 

**Node** 有如下方法:

- 构造函数: 

```python
__init__(node_name, namespace=None, context=None, cli_args=None, parameter_overrides=None, allow_undeclared_parameters=False, automatically_declare_parameters_from_overrides=False)
```

- 创建发布和订阅者:

```python
create_publisher(msg_type, topic, qos_profile) # qos: Quality of Service (QoS)
create_subcription(msg_type, topic, callback, qos_profile)
```

qos 参数一般设置为 10, 指节点消息队列的最大长度(通常代表其处理能力).

- 创建服务和客户端:

```python
create_service(srv_type, srv_name, callback, qos_profile=QoSProfile())
create_client(srv_type, srv_name, qos_profile=QoSProfile())
```

- 参数管理:

```python
declare_parameter(name, value=None, descriptor=None)
get_parameter(name)
set_parameters(parameters)
list_parameters(prefixes, depth)
```

- 日志:

`get_logger()` 获取与节点关联的日志记录器

- 销毁实体:

```python
destroy_publisher(publisher)
destroy_subscription(subscription)
destroy_service(service)
destroy_client(client)
destroy_node()
```

### ros 节点执行流程: 

```python
def main(args=None): 
	rclpy.init(args=args) # 初始化
	node = myNode() 
	rclpy.spin(node) # 循环执行, 保证节点持续存在
	node.destroy_node() 
	rclpy.shutdown() 
```

### 创建一个新 ROS2 包

创建一个新包模板:
```shell
cd /path/to/src
ros2 pkg create --build-type ament_python my_pkg
```

完善 `package.xml` 基本包信息:

```xml
<description>Examples of minimal publisher/subscriber using rclpy</description>
<maintainer email="you@email.com">Your Name</maintainer>
<license>Apache License 2.0</license>
```

在 ros pkg 中添加依赖, 须同时修改 `packages.xml`: 告诉 colcon 其环境依赖.

```xml
<exec_depend>rclpy</exec_depend>
<exec_depend>std_msgs</exec_depend>
```

修改 `setup.py`, 基本信息和 `package.xml` 严格相同. 

添加节点入口 `entry_points`, 告诉编译器:　该包中子程序的执行入口在何处.

```python
entry_points={
        'console_scripts': [
                'talker = py_pubsub.publisher_member_function:main',
                'listener = py_pubsub.subscriber_member_function:main',
        ],
},
```

`setup.cfg` 是自动添加的, 它将告诉 python 将执行程序放在 workspace 的 lib 文件夹中. `ros run` 会在 lib 中查找可执行程序.