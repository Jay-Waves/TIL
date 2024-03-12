## Network Info

### `ip`

`ip addr show`

### `nslookup` 

根据域名查看 ip.

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

## Network Debug

### `ping`

Pings host and outputs results.  

```bash
ping host
```

### `netstat`

检查哪些进程在监听端口:
```bash
netstat -lntp
```

### `wireshark`, `tshark`

[`wireshark`](https://wireshark.org/)，[`tshark`](https://www.wireshark.org/docs/wsug_html_chunked/AppToolstshark.html) 和 [`ngrep`](http://ngrep.sourceforge.net/) 可用于复杂的网络调试

## Network Connection Tool

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

请查看: [Network/应用层/ssh](../../Network/应用层/ssh.md)

### `curl`, `wget`

查看 [Networl/Tool/curl](../../Network/Tool/curl.md), 或者试试更潮的 [httpie](https://github.com/jkbrzt/httpie)

<pre>
$ wget file_url
</pre>

