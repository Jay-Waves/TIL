`DIR [path:][path][filename] [/parameters]`
其中文件路径可以隐藏，默认为pwd。也支持文本正则匹配
#dir 
## 常用参数
1. /a[:drhas- ] 显示具有指定属性的文件
- d 目录
- r 只读类型文件
- h 隐藏文件 
- s 系统文件
- `-`表示“否”的前缀（即不显示该类型）
2. /b 只显示名字，无信息和摘要
3. /o[:nsedg] 按顺序排序显示
- n 按字母表顺序
- s 按大小（小到大）
- `-`颠倒顺序的前缀
- e 按扩展名
- 按日期、时间（从先到后）
4. /p 每行信息暂停
5. /s 显示指定目录和所有**子目录**的文件
6. /t[:caw] 控制时间域（用何种时间排序）
- c 创建时间
- a 上次访问时间
- w上次写入时间

## 获取当前路径
linux的bash中可以使用pwd
cmd中则使用 #chdir 

## 应用
### 统计当前文件下所有文件的名称
`dir *.* /B > list.txt`
将所有信息都统计到list.txt，用来收集文件时配合excel查找非常好统计人数。
`dir *.pdf /B > list.txt`
统计所有pdf文件的名称