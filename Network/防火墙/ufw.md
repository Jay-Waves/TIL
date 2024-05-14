

## ufw 使用

- `sudo ufw enable`
- `sudo ufw disable`

规则:
- `sudo ufw allow ssh`
- `sudo ufw allow 22/tcp` 允许端口 22 的 tcp 报文.
- `sudo ufw allow from 192.168.1.10 to any port 22`
- `sudo ufw deny 1234/tcp`
- `sudo ufw delete [rule_number]`, rule_number 通过 `status` 命令查看.**