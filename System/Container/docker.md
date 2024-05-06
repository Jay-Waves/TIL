## 核心概念

### 容器 Container

容器是镜像的运行实例, 可以被动态地启动/停止/移动/删除, 容器删除后数据也会丢失, 所以一般将host的某个卷挂载到容器. 

容器类似虚拟机, 和host隔离, 有自己的文件系统/网络配置/进程空间.

### 镜像 Image

轻量可执行的独立软件包, 包括*代码, 运行时, 库, 环境变量和配置文件*. 

镜像内容由多个层次构成 (每层代表dockerfile一个指令), docker利用层次结构来重用层次, 以加速部署和节省空间. 镜像一旦构建完, 就不可变; 构建构成中, docker 会逐层构建, 每个层基于已构建层次, 在独立临时容器中构建, 并为已构建的层次生成缓存, 方便出错时快速重新构建. 修改 dockerfile 后, 修改应只影响当前层和之后的层, 不影响之前的层. 镜像实际就是多个层的组合, 所以多个镜像也可以类似的组合在一起.

如大型项目, 将各个构建部分拆分后, 构建为多个 docker images, 然后仅设计这些 images 间的依赖关系即可. 举例而言, navigation2 image 基于 ros2 image, ros2 image 基于 ubuntu image. docker 不仅构建时能够重用镜像, 运行时也可以直接连接多个docker.

清理镜像:
- `docker image prune`
- `docker system prune`

#### docker 实现机制

**层次构建**: Docker 构建时, 如果某一层需要修改前一层的文件, 该文件会被复制到当前层 (写时复制, copy-on-write, CoW), 当前层存在一个 OverlayFS 来记录这些改动, 保证修改不会影响到前一层, 这也意味着除非重新构建否则无法删除之前层的缓存. 而容器运行时有 UnioinFS 来将分层文件系统组合为统一文件系统, 让FS对容器内进程透明和连续. 镜像层次和命令层次是类似的, 也可以复用.

**容器分离**: 前一层次一旦被构建好, 就不可被更改, 除非重新构建. 镜像被放入容器内运行时, docker会在素有只读层上添加一个可写层, 所有(持久)写入操作都发生在这个容器层上, 而镜像是只读的. 基于此, Docker 的各个容器彼此隔离, 有独立的运行环境/文件系统/网络配置/进程空间, 允许为同一镜像创建多个容器实例. 由于共享了同样的镜像层, 节省了磁盘空间

**轻量级并发**: 容器们共享宿主机操作系统内核, 并且可以共享镜像层. 所以启动时间较快, 并且磁盘空间也占用较小.

***

## 网络

docker 默认网桥模式: 虚拟一个网桥(docker0), 为每一个容器分配一个IP地址 (container ip), 网桥是每个容器默认网关

| 网络模式  | 配置                           | 说明                                                                                                  |
| --------- | ------------------------------ | ----------------------------------------------------------------------------------------------------- |
| host      | --network host                 | 容器和宿主机共享 Network namespace. **注意,共享网络不等于共享代理**                                                                    |
| container | --network container:NAME_OR_ID | 容器和另外一个容器共享 Network namespace                                                              |
| none      | --network none                 | 容器有独立的 Network namespace，但并没有对其进行任何网络设置，如分配 veth pair 和网桥连接，配置 IP 等 |
| bridge    | --network                      | bridge 默认模式, 网络隔离, 但分配一个内部IP. 构建时也使用该网络模式.                                                                                       |

直接将宿主机设置为proxy即可, 如用[clash](../../Network/防火墙/代理.md), 设置 `all_proxy=http://host_ip:7890`

ps: git 不走系统代理, 构建时也要设置 proxy.

隔离网络:
```shell
# 用户自定义网络时, 默认和宿主网络是隔离的
docker network create <my_network>

docker run ... --network <my_network>
```

## 文件系统

### 卷 Volume

用于持久化数据, 即使容器被删除, 卷也会保留. 卷的位置由docker管理, 一般在 `/var/lib/docker/volumes` 下, 适合迁移.
```shell
docker volume create <my-volume>

docker run ... -v <my-volume>:/path/to/container/dir 
```

### 绑定挂载 Bind Mount

将主机上目录挂载到容器的特定目录, 用于多容器和宿主机共享.
```shell
docker run ... -v /path/to/host/dir:/path/to/container/dir
```

## 基础指令

docker 指令结构: `docker <func> <params>` 
- docker image ...
- docker container ...
- docker volume ...
- docker network ...

### 管理容器和镜像

拉取 DockerHub 镜像: docker pull

删除镜像: delete mounted containers -> delete image

### 运行

`docker run --name <container_name> <image_name>`

- docker run
- docker ps : 运行中容器进程

docker run:
- `-d` 后台运行
- `-it` 交互终端运行

容器多开:
```shell
# 交互模式
docker exec -it <my-container> /bin/sh

# 快速执行命令
docker exec <my-container> ls
```

***

## Dockerfile

dockfile 定义了 docker engine 如何构建一个 docker 镜像, 构建过程为root用户. 可以参考我为 Rozz 设计的 [rozz dockerfile](../Robotic%20OS/fuzz.project/nav2/rozz%20dockerfile.md)

- `ENV`: 镜像构建和执行过程中都会存在参.
- `ARG`: 仅镜像构建中存在的参数.
- `RUN`: 执行命令. 默认使用 `/bin/sh -c`, ubuntu 指向 dash.

目录变化如下:
- docker镜像存储在本地 docker hub (/var/docker??)
- log...

dockerfile 构建技巧 (方便构建时 debug):
- 易改动指令放在后面, 声明参数等不易改动指令放在前面. 
- 尽量RUN指令合并多个命令, 比如将多个安装命令合并, 可以一次清理更多文件.
- 每个层次中, apt update 更新的源列表, 应该在使用完后立刻删掉, 来减小镜像体积. 因为本层不删除, 后续曾就无法删除(虽然只存储一次), 但是这样也会造成构建缓慢的问题.
- 注意, 宿主机构建命令的不同, 也会导致 dockerfile 重新构建

#### 构建 dockerfile

`-t repos/image:tag`

```shell
docker build -t <image_name> .

docker run -it --name <coutainer_name> <image_name>

docker build -r /path/to/my/dockerfile .
```