image
- rozz/nav2_env : 没编译, 有 nav2 所需依赖. ros2-humble. ros2-jammy
- rozz/nav2_runtime

image 文件结构:
- /root/nav2_ws, 
- /root/nav2_ws/src/navigation --> /host/~/src/navigation2
- /root/rozz
- /root/log --> /host/~/rozz/log
- /root/src/navigation2 --> https://github.com/ros-planning/navigation2, 这个只用来安装依赖

nav2 dependency (not compiled, without src code): 

`sudo docker build -t rozz/nav2_env:base . --network=host`

```dockerfile
# ros humble, based on jammy.
# see: https://github.com/docker-library/repo-info/blob/master/repos/ros/remote/humble.md
FROM ros@sha256:6bdb91d8f1c7177afb834737d281caf3515965ebc24053eab04044f97568eadb

# no interactive install
ENV DEBIAN_FRONTEND=noninteractive

# install build and debug tools
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3-colcon-common-extensions \
    python3-pip \
    lcov \
    clang \
    libasan5 \
    curl \
    software-properties-common\
    && pip install \
    gcovr \
    && rm -rf /var/lib/apt/lists/*

# setup proxy, docker lan host ip
ARG PROXY="http://127.0.0.1:7890"
ARG http_proxy=$PROXY
ARG https_proxy=$PROXY
RUN git config --global http.proxy $PROXY
RUN git config --global https.proxy $PROXY

# add ros2 humble apt lists
RUN curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg \
        && echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | tee /etc/apt/sources.list.d/ros2.list > /dev/null

############## build nav2, nav2 as overlay, ros2 as underlay
# setup workspace
RUN mkdir -p /root/nav2_ws/src
RUN mkdir -p /root/src
RUN mkdir -p /root/rozz_ws/src
WORKDIR /root/src
# clone source code
RUN git clone https://github.com/ros-planning/navigation2 --branch humble ./navigation2

# source underlay of ros2 humble
ARG SOURCE_UNDERLAY='source /opt/ros/humble/setup.bash'

# install navigation2 dependencies
ARG http_proxy=''
ARG https_proxy=''
RUN /bin/bash -c "$SOURCE_UNDERLAY \
    && add-apt-repository universe \
    && apt-get update --fix-missing\
    && rosdep install -y --from-paths . --ignore-src\
    && rm -rf /var/lib/apt/lists/*"
```

basic compile

`sudo docker build -t rozz/nav2_runtime:base -f Dockerfile.nav2_runtime /home/Jay-Waves/src`

复制源码的方式, 允许我们不断对源码做出修改, 并重新编译为新镜像. 此处传入的路径为dockerfile的上下文路径, 可以用`.`直接访问.

```dockerfile
FROM rozz/nav2_env:base
WORKDIR /root/nav2_ws
# compiler flags
ENV CC=/usr/bin/clang
ENV CXX=/usr/bin/clang++
ARG FLAGS="-w -Wno-error  -Wno-inconsistent-missing-override -Wno-unused-but-set-variable"
# ENV MAKEFLAGS='-j4'

COPY ./navigation2 /root/nav2_ws/src/navigation2
RUN /bin/bash -c 'source /opt/ros/humble/setup.bash \
                && colcon build \
                        --symlink-install \
                        --parallel-workers 3 \
                        --cmake-args \
                                -DBUILD_TESTING=OFF \
                                -DCMAKE_CXX_FLAGS="$CMAKE_CXX_FLAGS $FLAGS" \
                                -DCMAKE_C_FLAGS="$CMAKE_C_FLAGS $FLAGS"'

# runtime environment
ENV GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:/opt/ros/humble/share/turtlebot3_gazebo/models
ENV TURTLEBOT3_MODEL=waffle
RUN echo "source /opt/ros/humble/setup.bash && source /root/nav2_ws/install/setup.bash" >> ~/.bashrc
```

asan/code coverage compile + runtime

`sudo docker build -t rozz/nav2_runtime:asan -f Dockerfile.nav2_asan_runtime /home/Jay-Waves/src`

```dockerfile
FROM rozz/nav2_env:base
WORKDIR /root/nav2_ws
# compiler flags
ENV CC=/usr/bin/clang
ENV CXX=/usr/bin/clang++

COPY ./navigation2 /root/nav2_ws/src/navigation2
# build without asan
ARG FLAGS="-w -Wno-error  -Wno-inconsistent-missing-override -Wno-unused-but-set-variable"
RUN /bin/bash -c 'source /opt/ros/humble/setup.bash \
                && colcon build \
                        --symlink-install \
                        --parallel-workers 3 \
                        --cmake-args \
                                -DBUILD_TESTING=OFF \
                                -DCMAKE_CXX_FLAGS="$CMAKE_CXX_FLAGS $FLAGS" \
                                -DCMAKE_C_FLAGS="$CMAKE_C_FLAGS $FLAGS"'

# build with asan somewhere, see https://github.com/GoesM/ROZZ_2/tree/main/nav2_for_llvm/ReadMe.md
ARG FLAGS="-w -Wno-error  -Wno-inconsistent-missing-override -Wno-unused-but-set-variable\
			-fsanitize=address -fno-omit-frame-pointer\
			--coverage -DCOVERAGE_RUN=1"
ARG ASAN_PACKS="nav2_amcl nav2_bt_navigator"
RUN /bin/bash -c 'source /opt/ros/humble/setup.bash && colcon build \
        --symlink-install \
        --parallel-workers 3 \
        --packages-above $ASAN_PACKS \
        --cmake-args \
                -DBUILD_TESTING=OFF \
                -DCMAKE_CXX_FLAGS="$CMAKE_CXX_FLAGS $FLAGS" \
                -DCMAKE_C_FLAGS="$CMAKE_C_FLAGS $FLAGS"'

# runtime environment
ENV ASAN_OPTIONS="new_delete_type_mismatch=1 detect_leaks=1 halt_on_error=0"
ENV GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:/opt/ros/humble/share/turtlebot3_gazebo/models
ENV TURTLEBOT3_MODEL=waffle
RUN echo "source /opt/ros/humble/setup.bash && source /root/nav2_ws/install/setup.bash" >> ~/.bashrc
```

-v /path/to/host/nav2/src:/root/nav2_ws/src

### 配置 宿主机和镜像间交互

让 docker 获取 宿主机的 rozz_2 工具:
```shell
docker run ... -v /path/to/host/rozz:/rozz/
```

docker 绑定挂载宿主机 rozz_2 的 log 目录. 

### 配置 ros2 进程间通信

`ROS_HOSTNAME`

`ROS_MASTER_URL`: `http://<docker-container-ip>:11311`

`ROS_IP

***

stash: 直接编译出一个可用的navigation2
```dockerfile
# ros humble, based on jammy.
# see: https://github.com/docker-library/repo-info/blob/master/repos/ros/remote/humble.md
FROM ros@sha256:6bdb91d8f1c7177afb834737d281caf3515965ebc24053eab04044f97568eadb

# no interactive install
ENV DEBIAN_FRONTEND=noninteractive

# install build and debug tools
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3-colcon-common-extensions \
    python3-pip \
    lcov \
    clang \
    libasan5 \
    curl \
    software-properties-common\
    && pip install \
    gcovr \
    && rm -rf /var/lib/apt/lists/*

# setup proxy, docker lan host ip
ARG PROXY="http://127.0.0.1:7890"
ARG http_proxy=$PROXY
ARG https_proxy=$PROXY
RUN git config --global http.proxy $PROXY
RUN git config --global https.proxy $PROXY

# add ros2 humble apt lists
RUN curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg \
        && echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | tee /etc/apt/sources.list.d/ros2.list > /dev/null

# build nav2, nav2 as overlay, ros2 as underlay
# setup workspace
RUN mkdir -p /root/nav2_ws/src
RUN mkdir -p /root/rozz
ENV OVERLAY_WS=/root/nav2_ws
WORKDIR $OVERLAY_WS
# clone source code
RUN git clone https://github.com/Jay-Waves/navigation2 --branch humble ./src/navigation2

# source underlay of ros2 humble
ARG SOURCE_UNDERLAY='source /opt/ros/humble/setup.bash'
# ARG SOURCE_OVERLAY='source $OVERLAY_WS/install/setup.bash'
# ENV COVERAGE_COMMAND="lcov --directory . --capture --output-file coverage.info"

# install navigation2 dependencies
ARG http_proxy=''
ARG https_proxy=''
RUN /bin/bash -c "$SOURCE_UNDERLAY \
    && add-apt-repository universe \
    && apt-get update --fix-missing\
    && rosdep install -y --from-paths src --ignore-src\
    && rm -rf /var/lib/apt/lists/*"

# compiler flags
ENV CC=/usr/bin/clang
ENV CXX=/usr/bin/clang++
ARG FLAGS="-w -Wno-error  -Wno-inconsistent-missing-override -Wno-unused-but-set-variable"
# ENV MAKEFLAGS='-j4'

# build
# if resources are limited, use --parallel-workers 2
RUN /bin/bash -c '$SOURCE_UNDERLAY && colcon build \
        --symlink-install \
        --parallel-workers 3 \
        --cmake-args \
                -DBUILD_TESTING=OFF \
                -DCMAKE_CXX_FLAGS="$CMAKE_CXX_FLAGS $FLAGS -Wno-unused-but-set-variable" \
                -DCMAKE_C_FLAGS="$CMAKE_C_FLAGS $FLAGS"'
```