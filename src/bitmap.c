#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errno.h"

#define BITS_PER_BYTE 8

struct bitmap {
    unsigned char *map;
    size_t capacity;
};

int bitmap_init(struct bitmap* bits, size_t nbits)
{
    size_t nbytes;

    bits->capacity = nbits;
    nbytes = (nbits + 7) / 8;

    bits->map = (unsigned char *)malloc(nbytes);
    if (!bits->map) {
        return -ENOMEM;
    }

    memset(bits->map, 0, nbytes);
    return 0;
}

void bitmap_destroy(struct bitmap *bits)
{
    if (bits) {
        free(bits->map);
        bits->capacity = 0;
    }
}

inline int hash(size_t n)
{
  return 1U << (n % 8);
}

/* 设置位 */
void bitmap_set(struct bitmap *bitmap, size_t n)
{
    if (n < bitmap->nbits)
        bitmap->map[n / 8] |= (1U << (n % 8));
}

/* 清除位 */
void bitmap_clear(struct bitmap *bitmap, size_t n)
{
    if (n < bitmap->nbits)
        bitmap->map[n / 8] &= ~(1U << (n % 8));
}

/* 测试位 */
int bitmap_test(const struct bitmap *bitmap, size_t n)
{
    if (n < bitmap->nbits)
        return bitmap->map[n / 8] & (1U << (n % 8));
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
