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

## usr info

### `finger`

Displays information about user.  

```bash
finger username
```

### `whoami`

Return current logged in username.

### `last`

Lists your last logins of specified user.  

```bash
last yourUsername
```

### `passwd`

Allows the current logged user to change their password.

### `id`

用户/组 ID 信息