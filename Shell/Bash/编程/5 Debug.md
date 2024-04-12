## 8 Debug

You can easily debug the bash script by passing different options to `bash` command. For example `-n` will not run commands and check for syntax errors only. `-v` echo commands before running them. `-x` echo commands after command-line processing.

```bash
bash -n scriptname
bash -v scriptname
bash -x scriptname
```

### 调试脚本

在脚本开头写上调试命令:

```bash
	set -euo pipefail
	trap "echo 'error: Script failed: see failed command above'" ERR
```

可以检测错误 (如管道错误, 变量未赋值), 并在错误发生时中断程序, 捕获 ERR/EXIT, 然后输出错误信息.