`.bat` 和直接执行 cmd 命令并不完全相同, 很多语法不兼容. (和 bash 不同).

```bat
@echo off
cd $Dir
File.exe arg
pause
```
- 注意中文要使用ANSI编码, 否则是乱码. 用notepad打开并另存为即可.

### %
1. `%var%` 变量引用， 如`path=%path%;c\hello`
2. `%[0-9]` 对形式参数的引用

### ^
转移字符, 常用来换行`end of line^ new line`

# 循环 `for`

### format
`FOR [parameter] %%variable IN (files or doc set)  DO command [command-parameters]`

**本质是批量在cmd中执行命令, 并在执行时将%var替换为对应内容.** 优点是即使报错也不会中止, 会继续执行下一个循环.
**NOTICE:** cmd中使用`%var`即可, 而.bat中需要使用`%%var`, 因为bat中需要对转义. 建议更多细节指南使用`help for`
> e.g. 'cmd
> `for %i in (f:\env\*) do echo %i` 打印目录下所有文件名和文件夹名

### parameter
- 文件夹循环 `/D`
匹配目录名而不匹配文件名.
*NOTICE:* (set)是一组文件或路径, 各个路径间拿逗号隔开即可. 如`(f:\env\python\*,f:\env\rust\*)`
> e.g. 'cmd
>`for /d %i in (f:\env\*) do echo %i` 循环该目录下的所有目录名, \*代指了很多文件. 注意cmd大部分命令不支持中间目录的通配符(只支持最后的通配符), 所以只能用循环实现. (所以说cmd是一坨屎)

- 目录树循环 `/R`
递归整个目录树执行for循环,可以**递归到最深层**至每一个角落. 指定的目录树root_path直接跟在/r参数后, 后面的(set)中路径可以使用root_path的相对路径.
>e.g. 'cmd
>`for /r f:\env %i in (*) do echo %i` 递归\env下所有

- 数字列表循环 `/L`
使用range(start, step, end)规定步长
>e.g. 'cmd
>`for /l %i in (1, 1, 5) do echo %i` 依次打印1~5


## 应用
### 得到命令结果
在linux下得到命令结果非常简单, 如`var = $(scripts)`
但cmd比较弱, 也没有变量概念, 这时候常使用`for /f`针对命令执行for, 来达到相同效果. 比如:

```bash
:'使用for /f获取当前目录路径; 这里注释用了bash语法, Obisdian不支持cmd高亮'
for /f %i in ('chdir | findstr path') do (echo %i)
```

### 删除子目录文件
下面是一个匹配并删除二级子目录下对应目录的例子.
`for /d %i in (D:\yjw\Notes\PyWay\DataScience_repo\*) do (for /d %d in (%i\*) do (del %d\translations && rmdir %d\translations))`