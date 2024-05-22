---
path: include/linux/mm_types.h
---

页是内核中基本的内存管理单元.

```c
struct page {
  unsigned long flags; // dirty, locked
  atomic_t _count; // reference count, -1 then page is free
  atomic_t _mapcount;
  unsigned long private;
  struct address_space *mapping;
  pgoff_t index;
  struct list_head lru;
  void *virtual; // pages's virtual address, NULL for high memory
};
```

区域指一组有类似硬件特征的页, 如:
- `ZONE_DMA`: DMA (Direct Memory Access) 区域, 用于硬件子系统 (网卡, 声卡) 直接访存, 而不经过 CPU. < 16MB
- `ZONE_DMA32` 仅支持 32 位设备的DMA区域.
- `ZONE_NORMAL`: 用于正常开辟和映射内存, 16MB~896MB.
- `ZONE_HIGH_MEMORY`: 装载高地址 (high memory), 大型系统中实际物理内存空间体积大于内核控制的虚拟内存空间体积, >896MB.

```c
struct zone {
  unsigned long watermark[NR_WMARK]; /* 水位线, 控制内存分配压力 */

  spinlock_t lock;
  struct free_area free_area[MAX_ORDER];
  
  spinlock_t lru_lock;
  struct zone_lru {
  struct list_head list;
  unsigned long nr_saved_scan;
  } lru[NR_LRU_LISTS];
  
  struct zone_reclaim_stat reclaim_stat;
  unsigned long pages_scanned; 
  
  unsigned long flags; /* dirty? */
  
  atomic_long_t vm_stat[NR_VM_ZONE_STAT_ITEMS];
  unsigned int inactive_ratio;

 /* wait table .... */
  
  struct pglist_data *zone_pgdat;
  unsigned long zone_start_pfn;
  unsigned long spanned_pages;
  unsigned long present_pages;
  
  const char *name;
};
```

### 开辟内存

开辟 `2^order` 连续内存页, 并返回指向第一个页的指针.

```c
struct page * alloc_pages(gfp_t gfp_mask, unsigned int order);

// allocate single page
struct page* alloc_pages(gfp_t gfp_mask);

// return a page filled with zeros. used before pages were given to 
// user space, because previous memory may've contained sensitive info.
unsigned long get_zeroed_page(unsigned int gfp_mask);

void free_pages(unsigned long addr, unsigned int order);
```

将页转换为对应逻辑地址.

```c
void *page_address(struct page *page);
```

开辟字节单位内存块.

```c
void *kmalloc(size_t, ...);

void kfree();
```

### GFP mask

GFP (get free page) mask flags, `gfp_t`, 用于
- 规定内核开辟内存的行为
- 规定内存区域的类型 (如 DMA, normal)

| Action Flags    |                                                                                                                                                                                                                                            |
| --------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `__GFP_WAIT`    | allocator can sleep                                                                                                                                                                                                                        |
| `__GFP_IO`      | allocator can start disk I/O                                                                                                                                                                                                               |
| `__GFP_FS`      | allocator can start filesystem I/O                                                                                                                                                                                                                                                                                                                                                                                                      |
| `__GFP_NOFAIL`  | allocator repeats the allocation until it succeeds                                                                                                                                                                                         |
| `__GFP_NORETRY` | allocator never retries if allocation fails                                                                                                                                                                                                |
| Zone Flags      |                                                                                                                                                                                                                                            |
| `__GFP_DMA`     | allocates only from `ZONE_DMA`                                                                                                                                                                                                                                                                                                                                                                                                                      |
| `__GFP_HIGHMEM` | allocates from `ZONE_HIGHMEM` or `ZONE_NORMAL`                                                                                                                                                                                             |
| Type Flags      | Zone + Action                                                                                                                                                                                                                                           |
| `GFP_ATOMIC`    | high priority and must not sleep. used in interrupt heandlers, in bottom halves                                                                                                                                                            |
| `GFP_NOIO`      | This allocation can block, but must not initiate disk I/O. This is the flag to use in block I/O code when you cannot cause more disk I/O, which might lead to some unpleasant recursion.                                                                                                                                                                                                                                           |
| `GFP_NOWAIT`    | Like `GFP_ATOMIC`, except that the call will not fallback on emergency memory pools. This increases the liklihood of the memory allocation failing.                                                                                                                                                                                                                                           |
| `GFP_KERNEL`      | This is a normal allocation and might block. This is the flag to use in process context code when it is safe to sleep. The kernel will do whatever it has to do to obtain the memory requested by the caller. Should be **default choice** |
| `GFP_USER`      | This is a normal allocation and might block. This flag is used to allocate memory for user space processes.                                                                                                                                |
| `GFP_DMA`       | This is an allocation from `ZONE_DMA`. Device drivers that need DMA-able memory use this flag, usually in combination with one of the preceding flags.                                                                                     |

```c
ptr = kmalloc(size, __GFP_WAIT | __GFP_IO | __GFP_FS);
```
