`df`，`mount`，`fdisk`，`mkfs`，`lsblk`

## Directory Management

### `cd`

change working directory.

### `ls`, `tree`

`ls -i`: inode

### `mkdir`

Makes a new directory.  

```bash
mkdir dirname
```

You can use this to create multiple directories at once within your current directory.

```bash
mkdir 1stDirectory 2ndDirectory 3rdDirectory
```

You can also use this to create parent directories at the same time with the -p (or --parents) flag. For instance, if you wanted a directory named 'project1' in another subdirectory at '/samples/bash/projects/', you could run:

```bash
mkdir -p /samples/bash/projects/project1
mkdir --parents /samples/bash/projects/project1
```

Both commands above will do the same thing.
If any of these directories did no already exist, they would be created as well.

### `pwd`

<pre>
$ pwd
/home/JayWaves/src
</pre>

### `ln`

`ln file1 file2` 创建硬连接  

硬链接缺点: 不能跨磁盘, 不能释放空间, 不能指向文件夹.
file2 -> file1 inode -> file1 data_block, 两者使用同一inode. 删除file1不影响file2

`ln -s file1 file2`创建软连接, file2文件类型为l

file2 -> file2 inode -> file2 data_block (l) -> file1 -> file1 inode -> file1 data_block.   
删除file1会使file2失效

## Disk

### `df`

Shows disk usage. (disk free)

### `du`

Shows the disk usage of files or directories. For more information on this command check this [link](http://www.linfo.org/du.html)

```bash
du [option] [filename|directory]
```

Options:

- `-h` (human readable) Displays output it in kilobytes (K), megabytes (M) and gigabytes (G).
- `-s` (supress or summarize) Outputs total disk space of a directory and supresses reports for subdirectories. 

Example:

```bash
du -sh pictures
1.4M pictures
```

### `quota`

Shows what your disk quota is.  

```bash
quota -v
```

### `ldparm`

SATA/ATA 磁盘更改以及性能分析.

## Index file

### `find` !

```bash
$ find . -name README.md
$ find . -iname REAME.md # case insensitive
$ find /home/user1 -name '*.png'
```

### `locate`

任意位置索引(需要先建立索引): `locate "pattern"`

## File Management

### `mv` !

Moves a file from one location to other.  

```bash
mv filename1 filename2
```

Where `filename1` is the source path to the file and `filename2` is the destination path to the file.

Also it can be used for rename a file.

```bash
mv old_name new_name
```

### `cp` !

Copies a file from one location to other.  

```bash
cp filename1 filename2
```

Where `filename1` is the source path to the file and `filename2` is the destination path to the file.

### `repren`

[`repren`](https://github.com/jlevy/repren) 用于批量重命名文件, 或在多个文件中搜索替换内容.

```sh
      # 将文件、目录和内容全部重命名 foo -> bar:
      repren --full --preserve-case --from foo --to bar .
      # 还原所有备份文件 whatever.bak -> whatever:
      repren --renames --from '(.*)\.bak' --to '\1' *.bak
      # 用 rename 实现上述功能（若可用）:
      rename 's/\.bak$//' *.bak
```

### `rm` !

删除文件. 常用 `-r` 递归删除文件夹, `-f` 强制删除(无确认)

```bash
rm filename
```

### `touch` !

Updates access and modification time stamps of your file. If it doesn't exists, it'll be created.

```bash
touch filename
```

Example:

```bash
$ touch trick.md
```

### `truncate`

`truncate` 用于创建[稀疏文件](https://zh.wikipedia.org/wiki/稀疏文件), 即高效创建空文件. `fallocate` (用于 ext4, xfs, btrf, ocfs2), `xfs_mkfile`, `mkfile` (solaris, mac os)

### security

managerment of files access permissions, see [security](security.md)