
### FAT

FAT 文件系统中有一个文件分配表 (FAT, File Allocation Table) 用于记录数据块 (簇, cluster) 的分配情况. 当磁盘容量较大时, 文件分配表本身占用的体积过大, 效率低. FAT32 单个文件大小限制在 4GB.

```c
/* FAT, File Allocation Table */
struct fat {
    uint32_t fat_table[NUM_CLUSTERS];
    struct dentry directory_entries[NUM_ENTRIES];
};

void read_file_allocation_table(struct fat *fs, uint32_t cluster_num) {
    uint32_t next_cluster = fs->fat_table[cluster_num];
    while (next_cluster != FAT_END_MARKER) {
        next_cluster = fs->fat_table[next_cluster];
    }
}

/*
将链表显式存储在 FAT 中, 从 1 开始的文件占用了索引: 1 3 4 6
1:  3
2:
3:  4
4:  6
5:
6: -1
*/
```