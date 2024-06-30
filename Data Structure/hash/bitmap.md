## 位图

位图 (bitmap), 用于记录**大量 0/1 状态**, 在 linux inode, afl 中有使用. 位图数据结构的核心是哈希映射算法, 即将键映射为位图范围内的一位.


```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct bitmap {
    unsigned char *blobs;
    size_t nbits; // sizeof(unsigned char) * 8 * capacity
};

/* 初始化位图 */
struct bitmap *bitmap_new(size_t nbits)
{
    struct bitmap *bitmap;
    size_t nbytes;

    bitmap = (struct bitmap *)malloc(sizeof(*bitmap));
    if (!bitmap)
        return NULL;

    bitmap->nbits = nbits;
    nbytes = (nbits + 7) / 8;

    bitmap->blobs = (unsigned char *)malloc(nbytes);
    if (!bitmap->blobs) {
        free(bitmap);
        return NULL;
    }

    memset(bitmap->blobs, 0, nbytes);

    return bitmap;
}

/* 释放位图 */
void bitmap_destroy(struct bitmap *bitmap)
{
    if (bitmap) {
        free(bitmap->blobs);
        free(bitmap);
    }
}

/* 设置位 */
void bitmap_set(struct bitmap *bitmap, size_t n)
{
    if (n < bitmap->nbits)
	    bitmap->blobs[n / 8] |= (1U << (n % 8));
}

/* 清除位 */
void bitmap_clear(struct bitmap *bitmap, size_t n)
{
    if (n < bitmap->nbits)
        bitmap->blobs[n / 8] &= ~(1U << (n % 8));
}

/* 测试位 */
int bitmap_test(const struct bitmap *bitmap, size_t n)
{
    if (n < bitmap->nbits)
        return bitmap->blobs[n / 8] & (1U << (n % 8));
    return 0;
}

```