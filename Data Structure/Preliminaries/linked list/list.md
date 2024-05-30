类**列表**形式的链表, 访问链表尾(末尾元素)速度为 `O(1)`, 访问链表头速度为 `O(n)`

```
list   last_entry                     first_entry
 v         v                              v
header -> node -> node -> node -> ... -> node -> NULL
```

```c
struct list_node { 
	int val; /* use `int` for type simplicity, `void*` is ok */
	struct list_node *next;
	// bool is_head; 
	// u32 ref_cnt;
};

struct list_node *list_new_entry(int val)
{
	struct list_node *entry;
	entry = (struct list_node *)malloc(sizeof(struct list_node));
	assert(list != NULL);
	entry->val = val;
	entry->next = NULL;
	return entry;
}

struct list_node *list_init() 
{
	return list_new_entry(NULL);
}

// 遍历链表节点
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != NULL; pos = pos->next)
	
int list_len(struct list_node *list)
{
	assert(list != NULL);
	int n=0;
	struct list_node *pos;
	list_for_each(pos, list)
		++n
	return n - 1;
}
```

> 应注意到, 该实现和**栈**结构十分类似.   
> 代码实现详见 [list.h](../../../appendix/程序/list.h) 以及 [list.c](../../../appendix/程序/list.c)

## push

```
          last_entry                     first_entry
              v                                v
header -> node(new) -> node -> node -> ... -> node -> NULL
```

```c
void list_push(int val, struct list_node *list)
{
	assert(list != NULL);
	struct list_node *entry = list_new_entry(val);
	entry->next = list->next;
	list->next = entry;
	return;
}
```

## pop

```
                      last           first
                        v              v
header -- node(out) -> node -> ... -> node -> NULL
```

```c
int list_pop(struct list_node *list)
{
	assert(list != NULL);
	assert(!list_is_clr(list));
	
	struct list_node *last = list->next;
	int val = last->val;
	list->next = last->next;
	free(last);
	return val;
}
```

## clear list

clear list means:
```
header -> NULL
```

```c
/*
 * delete the entire list, but header existed.
 */
void list_clr(struct list_node *list)
{
	assert(list != NULL);
	if (list_is_clr(list)) return;
	
	struct list_node *pos = list->next; // donot free header
	struct list_node *next = pos->next;
	while (pos != NULL) {
		// free(pos->val);
		free(pos);
		pos = next;
		next = pos->next; 
	}
	list->next = NULL;
}

bool list_is_clr(struct list_node *list)
{
	assert(list != NULL);
	if (list->next == NULL)
		return True;
	return False;
}
```