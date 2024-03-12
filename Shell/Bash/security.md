[权限与文件信息](../../Linux/File%20System/权限与文件信息.md)

#### 权限判定
1. 判定是否是*属主*, 如是, 赋予权限, **结束判定**
2. 判定是否是*属组*, 如是, 赋予权限, **结束判定**
3. 赋予*其他*权限

### 进程权限

文件权限位`rwx rwx rwx`

进程权限位`ugs rwx rwx rwx`

`--s`
提权问题

### super user

使用`su`命令进入管理员模式(`#`)

如果无法使用su, 那么需要初始化root密码: `sudo passwd`

使用`exit`退出管理员模式使用`su`命令进入管理员模式(`#`)

如果无法使用su, 那么需要初始化root密码: `sudo passwd`

使用`exit`退出管理员模式

### `chmod` !

The chmod command stands for "change mode" and allows you to change the read, write, and execute permissions on your files and folders. For more information on this command check this [link](https://ss64.com/bash/chmod.html).

将某个文件夹递归设置权限: `chmod -R 777 path`

设置新建文件权限: `umask 000`, umask 是一个权限掩码, 请求的权限码和 umask 进行按位与非, 生成最终权限码 (即 umask 中定义的权限被移除. 仅在临时会话有效. 系统默认 `umask 022`, 在 /etc/profile 中.

```bash
chmod -options filename
```

### `chown` !

The chown command stands for "change owner", and allows you to change the owner of a given file or folder, which can be a user and a group. Basic usage is simple forward first comes the user (owner), and then the group, delimited by a colon.

```bash
chown -options user:group filename
```
