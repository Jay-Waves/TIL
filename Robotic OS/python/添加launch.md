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