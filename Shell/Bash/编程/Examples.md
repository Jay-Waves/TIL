对文本进行组合操作:

```bash
	sort a b | uniq > c   # c 是 a 并 b
	sort a b | uniq -d > c   # c 是 a 交 b
	sort a b b | uniq -u > c   # c 是 a - b
```

检查目录下所有文件的内容:

```bash
	grep . *     # 即匹配目录下的任意文件内容, 按行输出
	head -100 *  # 按文件输出
```

sh 处理文本行列极便捷. 如计算文本文件第三列之和:

```bash
	awk '{ x += $3 } END { print x }' myfile
```

查看文件树+详细文件信息: (类似递归版的 `ls -l`)

```bash
	ls -lR
# or:
	find . -type f -ls
```

access.log 是 web 服务器的日志文件, 某个确定的值仅出现在某些行中 (如 acct_id 参数总在 URI 中). 计算出每个 `acct_id` 值有多少次请求:

```bash
egrep -o 'acct_id=[0-9]+' access.log | 
	cut -d= -f2 | 
	sort | 
	uniq -c | 
	sort -rn
```

持续检测文件改动:

```bash
# 监控某文件夹中文件改变
watch -d -n 2 'ls -rtlh | tail'

# 排查 WiFi 设置故障时, 监控网络设置的任何更改:
watch -d -n 2 ifconfig
```

解析 Markdown 文件, 并随机抽取项目

```bash
function taocl() {
	curl -s $myurl |
		pandoc -f markdown -t html |
		iconv -f 'utf-8' -t 'unicode' |
		xmlstarlet fo --html --dropdtd |
		xmlstarlet sel -t -v "(html/body/ul/li[count(p)>0])[$RANDOM mod last()+1]" |
		xmlstarlet unesc | fmt -80
}
```

递归地删除日志文件:

```bash
fint . -name "*.exe"  -type f -print -exec rm -rf {} \\;

ls | grep %*.log% | xargs rm
```

若删除了某文件, 用 `du` 检查内存未释放, 可以检查该文件是否被进程占用:

```bash
lsof | grep deleted | grep "filename-of-my-big-file"
```