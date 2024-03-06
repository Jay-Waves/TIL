## Network Operations

| ping | dig | whois | scp  | rsync |
| ---- | --- | ----- | ---- | ----- |
| ssh  | w   | wget  | curl | ip    |
| netstat     |     |       |      |       |

### `ip`

`ip addr show`

### `nslookup` 

根据域名查看 ip.

### `ping`

Pings host and outputs results.  

```bash
ping host
```

### `dig`

Gets DNS information for domain.  domain info get.

```bash
dig domain
```

### `whois`

Gets whois information for domain.  

```bash
whois domain
```

### `scp`

Transfer files between a local host and a remote host or between two remote hosts.

*copy from local host to remote host*

```bash
scp source_file user@host:directory/target_file
```

*copy from remote host to local host*

```bash
scp user@host:directory/source_file target_file
scp -r user@host:directory/source_folder target_folder
```

This command also accepts an option `-P` that can be used to connect to specific port.  

```bash
scp -P port user@host:directory/source_file target_file
```

### `rsync`

Does the same job as `scp` command, but transfers only changed files. Useful when transferring the same folder to/from server multiple times.

```bash
rsync source_folder user@host:target_folder
rsync user@host:target_folder target_folder
```

### `ssh`

详见: [ssh](../../Network/应用层/ssh.md)

ssh (SSH client) is a program for logging into and executing commands on a remote machine.  

```bash
ssh user@host
```

This command also accepts an option `-p` that can be used to connect to specific port.  

```bash
ssh -p port user@host
```

### `w`

Displays who is online.

### `wget`

Downloads file.  

```bash
wget file
```

### `curl`

Curl is a command-line tool for requesting or sending data using URL syntax. Usefull on systems where you only have terminal available for making various requests.

```bash
curl url
```

Use  `-X` or `--request` to specify which method you would like invoke (GET, POST, DELETE, ...).
Use `-d <data>` or `--data <data>` to POST data on given URL.

### `netstat`

检查哪些进程在监听端口:
```bash
netstat -lntp
```