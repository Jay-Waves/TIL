## 重定向

**stdout 重定向**:

覆盖写入
```bash
echo "Hello, World!" > output.txt

# 丢弃所有标准输入
echo "..." > /dev/null
```

附加写入, 而不是覆盖
```bash
echo "hello, world!" >> output.txt
```

**stderr 重定向**:

```bash
ls not_existing_file 2> error.log
ls not_existing_file 2>> error.log
```

同时重定向 stderr + stdout:

```bash
ls not_exisiting_file &>> output.log

# 将 stderr 重定向至 stdout 的地址
cat not_existed_file >logfile 2>&1
```

**stdin 重定向**: 默认 stdin 是输入设备.

```bash 
sort < unsorted.txt

# 关闭标准输入, 立即返回 EOF, 用于确保后台命令不再期望读入输入, 
# 而立即收到ROF关闭句柄
cmd < /dev/null

# <<< 将字符串直接作为命令的标准输入
grep "pattern" <<< "This is search term in string"
```

### here documents

bash 中一种重定向标准输入方法, 可以在命令行直接提供多个行的输入.

```bash
kubectl apply -f - <<EOF
apiVersion: v1
kind: Pod
metadata:
  name: mypod
  labels:
    app: myapp
spec:
  containers:
  - name: mycontainer
    image: nginx
    ports:
    - containerPort: 80
EOF
```

"here documents" 会展开 Shell 变量. 可以通过 `<<'EOF'` 禁用.

```bash
cat <<EOF > lab.cc
#include <stdio.h>
static long num_steps = 100000000;
double step, pi;
int main() {
	...
	return 0;
}
EOF
```

## 管道

管道 `|` 底层使用 Unix pipe 系统调用(一种进程间通信, IPC方式), sh 在两个命令之间建立一个缓冲区, 存储前一个命令的标准输出.

```bash
# 统计行数
find . -type f -name "*.txt" | wc -l
# 排序并去重
cat file.txt | sort | uniq
```

### `tee`

用于保留一份 stdout 的副本, 同时不干扰正常 stdout 输出.

```bash
# 保留 stdout 副本到多个文件, 同时屏幕仍有 cmd1 输出
cmd1 | tee file1 file2 file3

# `cmd1 | cmd2`, 同时将 cmd1 输出附加到 file1 中
cmd1 | tee -a file1 | cmd2

# 修改 /etc/ocnfig.conf, 同时输出修改内容
echo "Some configuration" | sudo tee /etc/someconfig.conf
```

## `xargs`

`xargs` 可以将字符串或 stdin 转化为命令参数:

```bash
echo "on two three" | xargs mkdir
# ls: ./on  ./two ./three
```

删除 `find` 命令找到的文件:

```bash
find . -name "*.txt" -type f | xargs rm
```

`-n` 指定每次命令执行时使用的参数个数. (输入多个参数时, xargs 实际在并发)

`xargs echo` 拿不准替换结果时, 可以使用先实验一下.

`-I` 指定一个替换字符串:
```bash
# 将 .txt 替换为 .bak
find . -name "*.txt" -type f | xargs -I {} mv {} {}.bak
```

`-0` + `find ... --null`: 当文件名中可能包含空格或特殊字符时, 与 `find ... -print0` 一起使用可以更安全地处理文件名, 多个文件名会被 `NULL` 隔离开而不是空格.

```bash
find . -name "*.txt" -type f -print0 | xargs -0 rm
```

对于循环中文件名中的特殊字符, 可以设置 `IFS='$\n'`. IFS, Internal Field Separator, 是 Bash 用来定义字段或数据项分隔符的特殊变量, 默认值包含 `space, \t, \n`, 在读取一行数据或展开一个数组时可能导致问题.

```bash
IFS=$'\n'
data="one two\nthree four"
echo -e "$data" | while read line; do
  echo "Line: $line"
done
unset IFS
```

