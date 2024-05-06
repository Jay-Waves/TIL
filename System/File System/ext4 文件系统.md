ext4以块组为单元, 块组结构如: `索引 (inode) 表` + `数据块`

ext4寻找数据的流程为 文件名 -> `目录项` -> inode号 -> `inode表` -> 指针 -> `数据块`

### 块组

```c
struct ext4_block_group { 
	struct ext4_superblock superblock; // 超级块 
	unsigned char block_bitmap[BITMAP_SIZE]; // 块位图 
	unsigned char inode_bitmap[BITMAP_SIZE]; // inode位图 
	struct ext4_inode inode_table[INODE_TABLE_SIZE]; // inode表 
	};
```

### inode表 

类似FCB, 记录文件信息.  
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
        // ... 以及一些更详细时间

};
```

inode记录问及那存储位置有多种方法: 
1. 连续分配, 记录 `起始位置`+`长度`.
2. 链接分配, 使用链表; 或指针索引表 (FAT), 按物理地址顺序, 记录下一逻辑地址对应的物理块
3. 索引分配, 按逻辑地址顺序, 记录对应物理块号.

unix系统中组合了上述方案:
```c
struct ext4_inode{
	// ...
	__le32 i_block[EXT4_N_BLOCKS];
	/* 
	iblock包括:
	前 `EXT4_NDIR_BLOCKS` 个直接引用块索引
	一级间接块索引
	二级间接块索引
	三级间接块索引
	*/
};
```

### 目录项

文件分为目录文件和一般文件两部分, 目录文件由下列目录项组成, 记录inode与文件名映射关系.

```c
struct ext4_directory_entry { 
	unsigned int inode_number; // inode号 
	char file_name[MAX_FILENAME_LENGTH]; // 文件名 
	};
```