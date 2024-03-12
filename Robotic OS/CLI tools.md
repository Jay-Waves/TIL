## ros bag

`ros bag` 工具用于记录订阅的消息

查看某订阅的消息: `ros2 topic echo <topic_name>`, 查看某订阅发布频率: `ros2 topic hz <topic_name>`

同时查看多个订阅信息, 并指定输出文件 `ros2 bag record -o <out_file_name> /turtle1/cmd_vel /turtle1/pose`

查看 bag 输出文件的详细信息: `ros2 bag info <out_file_name>`

将输出文件导入: `ros2 bag play <out_file_name>`

