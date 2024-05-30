ros gazebo 可以构建一个模拟环境, 用于测试机器人. ros 有相当多的地图文件 (`.world`) 可用, 比如 [Gazebo Maps Dataset](https://github.com/mlherd/Dataset-of-Gazebo-Worlds-Models-and-Maps)

```shell
# 设置机器人模型, 如 burger, waffle, waffle_pi
export TURTLEBOT3_MODEL=waffle

# 
export GAZEBO_MODEL_PATH=.gazebo/models/small_house/models/
gazebo small_house.world
```