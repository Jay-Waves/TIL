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