## User Management

修改主机名 `sudo hostnamectl set-hostname <host_name>`

切换用户 `su [- username]`, 加入 `-` 会使切换后的环境与使用该用户登陆后的环境相同, 省略 `username` 默认登录 root.

### 更改用户名

1. `su`
2. `vim /etc/passwd`
3. `vim /etc/shadow`
4. `vim /etc/group`

删除用户 `sudo userdel -r <usr_name>`

修改用户名: (最好使用临时sudo用户进行操作)
- `sudo usermod -l <new_usr> -m <old_usr>`
- `sudo groupmod -n <new_group> <old_group>`
- `sudo mv /home/<old> /home/<new>`, 并修改 `/etc/passwd` (如果未自动修改)

### 创建新用户

创建新用户: `sudo useradd -m -s /bin/bash <usr_name>`
- `-m` 创建用户家目录
- `-s /bin/bash` 默认 shell

设置新用户密码: `sudo passwd <usr_name>`

为新用户添加sudo权限: `sudo usermod -G sudo <usr>`

### 使用 root 用户

使用`su`命令进入管理员模式(`#`)

如果无法使用su (即无法使用 root), 那么需要初始化root密码: `sudo passwd`

使用`exit`退出管理员模式使用`su`命令进入管理员模式(`#`)

如果无法使用su, 那么需要初始化root密码: `sudo passwd`

使用`exit`退出管理员模式

### 将用户添加到 wheel 组

sudo 是一个提权的程序, 不同系统用不同用户组来标识用户的调用 sudo 权限.

在 Debian 系统版本中, 通过 sudo 用户组来控制用户 sudo 权限, 这些系统预设了 `etc/sudoers` 的用户组权限设置.

```
%sudo ALL=(ALL:ALL) ALL
```

基于 Arch 和 Fedora 的发行版, 则更为传统地使用 wheel 用户组而不是 sudo. 但这些系统一般不默认开启 `etc/sudoers` 中的设置.

```
$ visduo
# %wheel ALL=(ALL) ALL
```

## usr info

### `finger`

`finger username` 展示用户信息

### `whoami`

展示当前登入用户的用户名.

### `last`

`list <username>` 展示该用户上次登入信息.

### `passwd`

修改密码

### `id`

用户/组 ID 信息