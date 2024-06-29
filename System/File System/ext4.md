
### 块组

ext4 以**块组**为管理单元, 结构如下:

```c

#define EXT4_BLOCK_SIZE 4096
#define BITMAP_SIZE EXT4_BLOCK_SIZE

struct ext4_block_group { 
	struct ext4_superblock superblock;            // 超级块
	struct ext4_block_group_desc bg_desc_table[]; // 块组描述符表
	unsigned char block_bitmap[BITMAP_SIZE];      // 数据块位图 
	unsigned char inode_bitmap[BITMAP_SIZE];      // inode 位图 
	struct ext4_inode inode_table[];              // inode 表 
};

struct ext4_superblock {
	uint32_t s_inodes_count;      // 文件系统中 inode 总数
	uint32_t s_blocks_count;      // 文件系统中块综述
	uint32_t s_free_blocks_count; // 空闲块数
	uint32_t s_free_inodes_count;
	uint32_t s_first_data_block;  // 第一个数据块位置
	uint32_t s_log_block_size;    // log_2(block_size)
};

struct ext4_block_group_desc {
	uint32_t bg_block_bitmap;    // 数据块位图所在块号
	uint32_t bg_inode_bitmap;    // inode 位图的块号
	uint32_t bg_inode_table;     // inode 表的块号
	uint16_t bg_free_blocks_count;
	uint16_t bg_free_inodes_count;
	uint16_t bg_used_dirs_count;
};
```


注意, 每个块组都包含较多全局冗余信息, 如超级快和块组描述符信息, 它们都是全局信息. 在 Ext2 后续版本中, 改为了稀疏存储. 位图则用于标识数据块或索引节点是否被占用. 

### 索引节点

inode长度固定, 不存储数据 (提供位置), 不存储文件名 (由目录项提供)

```c
struct ext4_inode {
        __le16  i_mode;         /* File mode */
        __le16  i_uid;          /* Low 16 bits of Owner Uid */
        __le32  i_size_lo;      /* Size in bytes, Low 32 bits */
        __le32  i_atime;        /* Access time */
        __le32  i_ctime;        /* Inode Change time */
        __le32  i_mtime;        /* Modification time */
        __le32  i_dtime;        /* Deletion Time */
        __le16  i_gid;          /* Low 16 bits of Group Id */
        __le16  i_links_count;  /* Links count */
        __le32  i_blocks_lo;    /* Blocks count */
        // ...

        __le32  i_block[EXT4_N_BLOCKS]; /* Pointers to blocks */
        __le32  i_file_acl_lo;  /* File ACL */
        __le32  i_size_high;    /* Size in bytes, High 32 bits */
        __le32  i_obso_faddr;   /* Obsoleted fragment address */
		//...
		
        __le16  i_checksum_hi;  /* crc32c(uuid+inum+inode) BE */
        __le32  i_crtime;       /* File Creation time */
        // ... 

};
```


Unix 系统存储数据块时, 混用多种索引方式:
```c
struct ext4_inode{
	// ...
	__le32 i_block[EXT4_N_BLOCKS];
/* 
iblock包括:
	直接引用块索引, EXT4_NDIR_BLOCKS < 10
	一级间接块索引
	二级间接块索引, 更大文件
	三级间接块索引, 更大文件
*/
};
```

### 目录项

文件分为目录文件和一般文件两部分, 目录文件由下列目录项组成, 记录inode与文件名映射关系.

```c
struct ext4_directory_entry { 
	unsigned int inode_number; // inode 号 
	unsigned int name_len;     // 文件名长度
	unsigned int file_type;
	char file_name[MAX_FILENAME_LENGTH]; // 文件名 
	};
```

```
inode    type     filename
1        d        .
2        d        ..
3        f        readme.txt
```

### 文件索引

文件名 --> 目录项 --> Inode 号 --> Inode 表 --> Inode --> 数据块

#### 硬链接

**硬链接**, Hard Link, 指多个目录项指向同一个索引节点, 该索引节点的被引用数增加. 该链接方法无法跨文件系统, 当所有硬链接和源文件被删除时, 源文件才会被彻底删除.

```
dentry  name        inode
1       myfile      101
2       hardlink1   101
3       hardlink2   101
```

#### 软链接

**软链接**, Soft Link, 创建一个新文件, 文件内容是被链接文件的路径. 访问该软链接时, 实际通过该路径访问到另一个文件. 软连接可以跨文件系统, 目标文件被删除时, 软链接失效.

```
dentry name       inode
1      myfile     101
2      softlink   103

inode blocks 
101   ...
103   /path/to/myfile
```