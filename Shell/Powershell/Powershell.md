寻求帮助:

- 参数: `-Help`
- 命令: `Get-Help <cmd> -Detailed`
- [官方文档](https://learn.microsoft.com/en-us/powershell/)

powershell 为 bash 和 cmd 用户准备了对应命令的别名.

补全 `cmd` 一栏对应的命令如下：

| zsh                      | pwsl                                                    | pwsl alias                  | cmd                     |
| ------------------------ | ------------------------------------------------------- | --------------------------- | ----------------------- |
| `grep`                   | `find-string -Pattern`                                  |                             | `findstr`               |
| `ls, dir`                | `get-childitem`                                         | `gci`                       | `dir`                   |
| `pwd`                    | `get-location`                                          | `gl`, `pwd`                 | `cd`                    |
| `mkdir`                  | `new-item -Type Directory`                              | `ni -Type Directory`, `pwd` | `mkdir`                 |
| `rm`                     | `remove-item`                                           | `ri`, `rm`, `del`           | `del` or `rmdir`        |
| `mv`                     | `move-item`                                             | `mi`, `mv`, `move`          | `move`                  |
| `cp`                     | `copy-item`                                             | `ci`, `cp`                  | `copy`                  |
| `grep`                   | `select-string`                                         | `findstr`, `sls`            | `findstr`               |
| `echo`                   | `write-output`                                          | `echo`                      | `echo`                  |
| `cat`                    | `get-content`                                           | `gc`, `type`, `cat`         | `type`                  |
| `chmod`                  | `set-acl`                                               |                             |                         |
| `export VarName="value"` | `$env:VarName="value"`                                  |                             | `set VarName=value`     |
| `clear`                  | `cls`                                                   |                             | `cls`                   |
| `cd .. && pwd`           | `cd; pwd`                                               |                             | `cd .. & cd`            |
| `ln -s`                  | `new-item -ItemType SymbolicLink -Path ... -Target ...` |                             | `mklink` or `mklink /D` |
|                          | `get-clipboard`                                         | `gcb`                       | `clip.exe` (not same)   |
| `ip`                     | `get-netipaddress`                                      |  None                           | `ipconfig`                        |


## Variable

pwsl 其实不区分大小写, 别被命令吓到了 (🖤)

```powershell
$MyVar = "hello, world!"

Write-Output $MyVar

Get-Type $MyVar

$homeDir = $env:USERPROFILE
```