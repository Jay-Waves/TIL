
## Trie

Trie, 也被称为前缀树, 字典树, 是一种索引树. 索引值为字符串, 但树中每个节点不存储完整的索引值, 而是存储 26 条边, 每个边代表一个字符 (a-z), 由树深度优先的路径来定义整个索引值. 每个节点能够定义一个字符串前缀, 因此被称为字典树.

```
      root
    /     \
   a        b   ...
 / | \    / | \
a  b  c  a  b  c
   |
  (ab)
```

- 避免了大量重复的字符串比较, 每个前缀只比较一次.
- 字典树时间复杂度**仅与字符串长度正相关**, 为 $O(\lceil\log_{k}{{n}\rceil})$ ($k$ 为字母表大小), 与字符串代表的值无关 $O(n)$.
- 与哈希表相比, Trie 无需精心挑选一个哈希函数. 性质良好的哈希函数复杂度也约为 $O(\lceil\log_{k}{{n}\rceil})$, 所以和字典树比无明显速度优势. 但字典树的键形式只能是类字符串.

### 实现

使用数组实现, `tree[i][j]` 表示节点 i 的第 j 个儿子节点的编号, `tree[]` 表示一个节点, 有 26 个子节点 `tree[i][]`, 表示字母 `a-z`. 根节点的编号为 0, 后续节点依次递增. 使用位图结构标识某个节点是否是一个单词结尾, 位图相关例程详见 [bitmap](../../hash/bitmap.md).


```c
#include <ctype.h>
#define ROOT 0
#define ALPHABET_SIZE 26
#define MAX_SIZE 10000000 // 10e7

struct trie {
	int size;     // number of nodes, int[ALPHABET_SIZE]
	int capacity; // capacity of nodes, int[ALPHABET_SIZE]
	struct bitmap *is_end;
	int *nodes;   // nodes[i][j] -> nodes[i * ALPHABET_SIZE + j]
}

void trie_new(struct trie *t)
{
	t->size = 0;
	t->capacity = MAX_SIZE;
	t->nodes = (int *) malloc( t->capacity * ALPHABET_SIZE * sizeof(int) );
	memset(t->nodes, 0, t->capacity * ALPHABET_SIZE * sizeof(int));
	t->is_end = bitmap_new( t->capacity ); 
}

// get index of the son `j` of parent `i`
inline void trie_get_id(int i, int j)
{
	return i * ALPHABET_SIZE + j;
}

void trie_insert(struct trie* t, char *str) 
{
	int len = strlen(str);
	int parent = ROOT;
	if (t->size == t->capacity) 
		return -ENOSPC;
	for (int i = 0; i < len; i++) {
		if (!isalpha(str[i]))
			return -EINVAL;
		int son = tolower(str[i]) - 'a'; 
		int idx = trie_get_id(parent, son);
		if (!t->nodes[idx])
			t->nodes[idx] = ++(t->size);
		parent = t->nodes[idx];
	}
	bitmap_set(t->is_end, parent);
}
```
