ros 具名版本下一般有如下打包类型:
- desktop: 相对完整, 包括rviz/rqt
- perception: 专注感知功能, 如图像和声音处理.
- gazebo: 开源机器人模拟器, 用于机器人算法开发和测试
- core: 最小安装, 仅有核心核心通信和消息传递
- base: 提供一些基本功能/动作库/插件库/更多驱动, 但不支持图形界面.

ros2 每年发布一个具名版本, 同时也引入了LTS版本.
- regular releases: 1 year
- long term support: 5 years

**ros各具名版本不保证向后兼容性, 升级版本需谨慎!**