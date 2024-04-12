检查依赖: `rosdep install -i --from-path src --rosdistro humble -y`

编译: `colcon build --symlink-install`
- 选择特定包编译: `--packages-select <pkg>`, 不编译其依赖.
- 选择特定包编译: `--packages-up-to <pkg>`, 编译其依赖.
- 已存在包, 重新编译, 包括相关依赖: `--packages-above <pkg>`
- `--allow-overriding <pkg>`, 忽略 underlay 的依赖文件冲突.
- `--parallel-workers 1`, 限制并行数. 搭配 `export MAKEFLAGS="-j 4"`.

测试: `colcon test`

构建: colcon 使用 `package.xml` 来指定依赖. 使用模板创建一个库: `ros2 pkg create`

## 构建 workspace

在 workspace 中置一个 `COLCON_IGNORE` 空文件, 来让 colcon 忽略该文件夹.

workspace 结构:
- `src`: 源码
- `build`: 中间文件, 通常是 make 工具激活的地方
- `install`: 
- `log`: 日志信息

构建 ros 第三方库:
1. clone
2. 检查库依赖: `rosdep install -i --from-path src --rosdistro humble -y`
3. 编译: `colcon build`, `--symlink-install` 参数可以防止 python 引用出错, 并且自动增量编译.
4. 导入配置 (overlay): `source install/local_setup.bash`. 

ros 有一个 layer 概念, ros 基于 linux, 所以 ros 是 overlay, linux 是其 underlay; 在 ros 基础上构建包, ros 是 underlay, 构建的包则是 overlay. setup 文件就是用来导入新的 overlay 环境的, overlay 会自动导入其 underlay.

### coverage

colcon 级编译参数. 见 [Add a mixin for GCC coverage flags by jpsamper2009 · Pull Request 8 · colconcolcon-mixin-repository](https://github.com/colcon/colcon-mixin-repository/pull/8)
```
colcon build --mixin coverage
```

> 详见 [colcon coverage](colcon%20coverage.md)

### 更多参数

- `--continue-on-error`
- `--cmake-clean-cache`

## 构建 package

包源码一般放在 workspace 的 src 文件夹的子文件夹中.

### 使用 cmake 构建

包内容:
- `CMakeLists.txt` file that describes how to build the code within the package
- `include/<package_name>` directory containing the public headers for the package
- `package.xml` 存储包的元数据, 如 license, description 等. [XML 格式见此](obsidian://open?vault=SoftWare&file=%E6%95%B0%E6%8D%AE%E5%BA%93%2F%E6%95%B0%E6%8D%AE%E6%A0%BC%E5%BC%8F%2FXML)
- `src` directory containing the source code for the package

目录结构:
```
my_package/
     CMakeLists.txt
     include/my_package/
     package.xml
     src/
```

创建包: `ros2 pkg create --build-type ament_cmake [--node-name <node_name>] <package_name> [--dependencies rclpy]` 其中 `dependencies` 参数可自动修改 package.xml 和 CMakeLists.txt.

编译包: `colcon build --packages-select my_package`

要是用包, 需要添加配置文件 (overlay): `source install/local_setup.bash`, 然后运行: `ros2 run <my_package> <my_node>`



### 使用 python 构建

包内容:
- `package.xml` 
- `resource/<package_name>` marker file for the package
- `setup.cfg` is required when a package has executables, so `ros2 run` can find them
- `setup.py` 知道如何构建包. 其中也有 `package.xml` 中的信息, 两者需严格相同.
- `<package_name>` - a directory with the same name as your package, used by ROS 2 tools to find your package, contains `__init__.py`

目录结构:
```
my_package/
      package.xml
      resource/my_package
      setup.cfg
      setup.py
      my_package/
```

创建: `ros2 pkg create --build-type ament_python <package_name>`

