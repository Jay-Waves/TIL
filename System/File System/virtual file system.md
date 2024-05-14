linux 虚拟文件系统 (VFS, virtual file system) 是接口层, 将[文件系统](文件系统.md)具体实现与操作系统服务剥离开. VFS 表示为统一的**树形**目录结构, 新文件系统通过**挂载 (mount)** 的方式装载到 VFS 的 `/mnt` 目录中. 

VFS 将不同文件系统实现的接口统一, 使 linux 具有支持不同环境的能力, 透明地实现内核功能. 定义和容纳具体文件系统的设备称为**块设备 (block device).**

VFS 在两个主要目标上做 trade-off:
1. 高效访问文件.
2. 确保文件数据正确.

```c
open()
write()
read()
close()
```

用户一般以字节为单位处理文件数据, 而操作系统以数据块为单位处理. FS 负责屏蔽这种差异.

inode: VFS inode holds information about a file or directory
```c
struct inode {
    kdev_t                       i_dev;
    unsigned long                i_ino; // number of inode
    umode_t                      i_mode; // file type and permission
    nlink_t                      i_nlink; // reference count
    uid_t                        i_uid; // user id
    gid_t                        i_gid; // group id
    kdev_t                       i_rdev;
    off_t                        i_size; // file size
    
    time_t                       i_atime; // access time
    time_t                       i_mtime; // modified time
    time_t                       i_ctime; // create tiem
    
    unsigned long                i_blksize; // size of block
    unsigned long                i_blocks;  // number of blocks
    unsigned long                i_version;
    unsigned long                i_nrpages;
    struct semaphore             i_sem;
    struct inode_operations      *i_op;
    struct super_block           *i_sb; // super block
    struct wait_queue            *i_wait;
    struct file_lock             *i_flock;
    struct vm_area_struct        *i_mmap;
    struct page                  *i_pages;
    struct dquot                 *i_dquot[MAXQUOTAS];
    struct inode                 *i_next, *i_prev;
    struct inode                 *i_hash_next, *i_hash_prev;
    struct inode                 *i_bound_to, *i_bound_by;
    struct inode                 *i_mount;
    unsigned short               i_count;
    unsigned short               i_flags;
    unsigned char                i_lock;
    unsigned char                i_dirt;
    unsigned char                i_pipe;
    unsigned char                i_sock;
    unsigned char                i_seek;
    unsigned char                i_update;
    unsigned short               i_writecount;
    union {
        struct pipe_inode_info   pipe_i;
        struct minix_inode_info  minix_i;
        struct ext_inode_info    ext_i;
        struct ext2_inode_info   ext2_i;
		 ...
        struct socket            socket_i;
        void                     *generic_ip;
    } u;
};
```

```c
struct dentry {
	atomic_t d_count;
	unsigned int d_flags; 
	struct inode *d_inode;
	struct dentry *d_parent;
	struct list_head d_child;
	struct list_head d_subdirs;
	struct qstr d_name;
	struct super_block *d_sb;
}
```

## inode operations