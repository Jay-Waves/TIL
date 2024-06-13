// hash_table.h
#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

typedef struct {
    char* key;
    char* value;
} ht_item;

typedef struct {
    int size;
    int count;
    ht_item** items;
} ht_hash_table;

static ht_item* ht_new_item(const char* k, const char* v);
ht_hash_table* ht_new();
static void ht_del_item(ht_item* i);
void ht_del_hash_table(ht_hash_table* int);

// hash functions
static int ht_hash(const char* s, int p, int m);
static int ht_get_hash(const char* s, int num_buckets, int attempt)

// methods
void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* h, const char* key);

#endif // _HASH_TABLE_H
