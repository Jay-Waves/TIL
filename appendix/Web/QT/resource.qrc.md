qt资源文件, 可以编译一下加快加载速度

`rcc -g python -o resource.py resource.qrc`

qss 可以应用于类名和objectName:

```
Dog > leg 仅应用于Dog中的直接leg组件

Dog 应用于Dog类

Dog, #mydog 还应用于ObjectName叫"mydog"的qt对象

Dog leg 应用于Dog中的所有leg组件, 包括间接组件

Dog leg#mydogleg 这个只能应用于ObjectName为mydogleg的组件
```