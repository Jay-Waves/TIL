## File Info

### `file`

确定文件类型

<pre>
$ file index.html
 index.html: HTML document, ASCII text
</pre>

### `cat`
  
  ```bash
  cat file1 file2 
  cat file1 file2 > combinedfile
  cat < file1 > file2           # copy file1 to file2
  cat > file                    # accept input from keyboard
  ```

### `diff`

Compares files, and lists their differences.  

```bash
diff filename1 filename2
```

标准的源代码对比及合并工具是 `diff` 和 `patch`。使用 `diffstat` 查看变更总览数据。注意到 `diff -r` 对整个文件夹有效。使用 `diff -r tree1 tree2 | diffstat` 查看变更的统计数据。`vimdiff` 用于比对并编辑文件。

### `wc`

Tells you how many lines, words and characters there are in a file. 

```bash
$ wc demo.txt
7459   15915  398400 demo.txt
```

Where `7459` is lines, `15915` is words and `398400` is characters.

- `wc -l`, lines
- `wc -m`, characters 
- `wc -w`, words
- `wc -c`, bytes

## View files

### `tail`

Outputs the last 10 lines of file. Use `-f` to output appended data as the file grows.  

```bash
tail filename
```

### `head`

Outputs the first 10 lines of file  

```bash
head filename
```

### `more`, `less`

Shows the first part of a file (move with space and type q to quit).  

```bash
more filename
```

## Compress Files

### `xz`

### `7zip`

### `tar`

### `gzip`

`gzip`

Compresses files.  

```bash
gzip filename
```

`gunzip`

Un-compresses files compressed by gzip.  

```bash
gunzip filename
```

`gzcat`

Lets you look at gzipped file without actually having to gunzip it.  

```bash
gzcat filename
```

`zless`、`zmore`、`zcat` 和 `zgrep` 也可对压缩文件进行操作。
