`df`，`mount`，`fdisk`，`mkfs`，`lsblk`

## Directory

### `cd`

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

## Management