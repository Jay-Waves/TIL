开启内置终端:
```vim
:Terminal
:term

" 关闭
^w :quit!
```

执行 shell 命令:

- 使用`:!command`,直接执行linux命令。如`:!date`
- 执行命令并将结果添加到光标处`:r !command`，注意有空格


