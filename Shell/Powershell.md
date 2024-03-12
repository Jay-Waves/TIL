寻求帮助:

- 参数: `-Help`
- 命令: `Get-Help <cmd> -Detailed`

创建符号链接:

```powershell
New-Item -ItemType SymbolicLink -Path "link\path" -Target "target\path"
```

powershell 为 bash 和 cmd 用户准备了对应命令的别名.

| zsh                      | pwsl                       | pwsl alias                  |
| ------------------------ | -------------------------- | --------------------------- |
| `grep`                   | `find-string -Pattern`     |                             |
| `ls, dir`                | `get-childitem`            | `gci`                       |
| `pwd`                    | `get-location`             | `gl`, `pwd`                 |
| `mkdir`                  | `new-item -Type Directory` | `ni -Type Directory`, `pwd` |
| `rm`                     | `remove-item`              | `ri`, `rm`, `del`           |
| `mv`                     | `move-item`                | `mi`, `mv`, `move`          |
| `cp`                     | `copy-item`                | `ci`, `cp`                  |
| `grep`                   | `select-string`            | `findstr`, `sls`            |
| `echo`                   | `write-output`             | `echo`                      |
| `cat`                    | `get-content`              | `gc`, `type`, `cat`         |
| `chmod`                  | `set-acl`                  |                             |
| `export VarName="value"` | `$env:VarName="value"`     |                             |
| `clear`                  | `cls`                      |                             |

## Variable

pwsl 其实不区分大小写, 别被命令吓到了 (🖤)

```powershell
$MyVar = "hello, world!"

Write-Output $MyVar

Get-Type $MyVar

$homeDir = $env:USERPROFILE
```