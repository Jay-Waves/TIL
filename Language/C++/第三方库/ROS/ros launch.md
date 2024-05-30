## launch

`ros launch` 允许通过配置文件运行节点


### 设置参数

命令行喂参数:
`ros2 run my_package my_node --ros-args --params-file path/to/my_params.yaml`

launch中喂参数:
```python
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='my_package',
            executable='my_node',
            parameters=['path/to/my_params.yaml']
        ),
    ])``
```

### 设置ros2的launch

c++ ros 包中, 使用 launch 需要在 cmakelist.txt 中明确安装:
```cmake
# install launch
# install(DIRECTORY launch/ DESTINATION share/${PROJECT_NAME}/)
```

colcon for python 需要设置 setup.py:

```python
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, '/launch', ['launch/silent_fuzz.py']),
        ('share/' + package_name, ['package.xml']),
    ],
```

并在 xml 添加依赖
```xml
<depend>launch</depend>
<depend>launch_ros</depend>
```

然后启动: 
`ros2 launch pkg_name launch.py`