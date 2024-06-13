## 位图

位图 (bitmap), 用于记录**大量 0/1 状态**, 在 linux inode, afl 中有使用. 位图数据结构的核心是哈希映射算法.



```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BITS_PER_BYTE 8

struct bitmap {
    unsigned char *map;
    size_t nbits;
};

/* 初始化位图 */
struct bitmap *bitmap_create(size_t nbits)
{
    struct bitmap *bitmap;
    size_t nbytes;

    bitmap = (struct bitmap *)malloc(sizeof(*bitmap));
    if (!bitmap)
        return NULL;

    bitmap->nbits = nbits;
    nbytes = (nbits + BITS_PER_BYTE - 1) / BITS_PER_BYTE;

    bitmap->map = (unsigned char *)malloc(nbytes);
    if (!bitmap->map) {
        free(bitmap);
        return NULL;
    }

    memset(bitmap->map, 0, nbytes);

    return bitmap;
}

/* 释放位图 */
void bitmap_destroy(struct bitmap *bitmap)
{
    if (bitmap) {
        free(bitmap->map);
        free(bitmap);
    }
}

/* 设置位 */
void bitmap_set(struct bitmap *bitmap, size_t n)
{
    if (n < bitmap->nbits)
        bitmap->map[n / BITS_PER_BYTE] |= (1U << (n % BITS_PER_BYTE));
}

/* 清除位 */
void bitmap_clear(struct bitmap *bitmap, size_t n)
{
    if (n < bitmap->nbits)
        bitmap->map[n / BITS_PER_BYTE] &= ~(1U << (n % BITS_PER_BYTE));
}

/* 测试位 */
int bitmap_test(const struct bitmap *bitmap, size_t n)
{
    if (n < bitmap->nbits)
        return bitmap->map[n / BITS_PER_BYTE] & (1U << (n % BITS_PER_BYTE));
    return 0;
}

/* 打印位图 - 调试用 */
void bitmap_print(const struct bitmap *bitmap)
{
    size_t i;
    for (i = 0; i < bitmap->nbits; ++i) {
        printf("%d", bitmap_test(bitmap, i) ? 1 : 0);
        if ((i + 1) % BITS_PER_BYTE == 0)
            printf(" ");
    }
    printf("\n");
}

/* 示例使用 */
int main(void)
{
    struct bitmap *bitmap;
    size_t nbits = 32;
    size_t i;

    bitmap = bitmap_create(nbits);
    if (!bitmap) {
        printf("Failed to create bitmap\n");
        return -1;
    }

    bitmap_set(bitmap, 1);
    bitmap_set(bitmap, 5);
    bitmap_set(bitmap, 31);

    printf("Bitmap after setting bits 1, 5, and 31:\n");
    bitmap_print(bitmap);

    bitmap_clear(bitmap, 5);
    printf("Bitmap after clearing bit 5:\n");
    bitmap_print(bitmap);

    for (i = 0; i < nbits; ++i) {
        printf("Bit %zu is %d\n", i, bitmap_test(bitmap, i));
    }

    bitmap_destroy(bitmap);

    return 0;
}

```