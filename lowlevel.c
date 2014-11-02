#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libdb.h"

//Функции для облегчения доступа к блоку
inline int get_key_size(Block block, int i)
{
	int sz_head = sizeof(struct NodeHead);
	int sz_data = sizeof(struct NodeData);
	int offset = sz_head + i * sz_data;
	return (int) ((struct NodeData *) (block + offset))->key_size;
}

inline void set_key_size(Block block, int i, int key_size)
{
	int sz_head = sizeof(struct NodeHead);
	int sz_data = sizeof(struct NodeData);
	int offset = sz_head + i * sz_data;
	((struct NodeData *) (block + offset))->key_size = key_size;
}

inline void * get_key(Block block, int i)
{
	int sz_head = sizeof(struct NodeHead);
	int sz_data = sizeof(struct NodeData);
	int offset = sz_head + i * sz_data;
	return ((struct NodeData *) (block + offset))->key;
}

inline void set_key(Block block, int i, void *key, int key_size)
{
	int sz_head = sizeof(struct NodeHead);
	int sz_data = sizeof(struct NodeData);
	int offset = sz_head + i * sz_data;
	memcpy(((struct NodeData *) (block + offset))->key, key, key_size);
}

inline int get_value_size(Block block, int i)
{
	int sz_head = sizeof(struct NodeHead);
	int sz_data = sizeof(struct NodeData);
	int offset = sz_head + i * sz_data;
	return (int) ((struct NodeData *) (block + offset))->value_size;
}

inline void set_value_size(Block block, int i, int value_size)
{
	int sz_head = sizeof(struct NodeHead);
	int sz_data = sizeof(struct NodeData);
	int offset = sz_head + i * sz_data;
	((struct NodeData *) (block + offset))->value_size = value_size;
}

inline void * get_value(Block block, int i)
{
	int sz_head = sizeof(struct NodeHead);
	int sz_data = sizeof(struct NodeData);
	int offset = sz_head + i * sz_data;
	return ((struct NodeData *) (block + offset))->value;
}

inline void set_value(Block block, int i, void *value, int value_size)
{
	int sz_head = sizeof(struct NodeHead);
	int sz_data = sizeof(struct NodeData);
	int offset = sz_head + i * sz_data;
	Block block_off = block + offset;
	memcpy(((struct NodeData *) (block_off))->value, value, value_size);
}

int get_n(Block block)
{
	return ((struct NodeHead *) block)->n;
}

void set_n(Block block, int n)
{
	((struct NodeHead *) block)->n = n;
}

int get_leaf(Block block)
{
	return ((struct NodeHead *) block)->leaf;
}

void set_leaf(Block block, int leaf)
{
	((struct NodeHead *) block)->leaf = leaf;
}

int get_num(Block block)
{
	return ((struct NodeHead *) block)->num;
}

void set_num(Block block, int num)
{
	((struct NodeHead *) block)->num = num;
}

int get_c(Block block, int i)
{
	int sz_head = sizeof(struct NodeHead);
	int sz_data = sizeof(struct NodeData);
	int offset = sz_head + i * sz_data;
	return (int) ((struct NodeData *) (block + offset))->c;
}
void set_c(Block block, int i, int c)
{
	int sz_head = sizeof(struct NodeHead);
	int sz_data = sizeof(struct NodeData);
	int offset = sz_head + i * sz_data;
	((struct NodeData *) (block + offset))->c = c;
}

void get_data(Block block, int i, struct DBT *key, struct DBT *value)
{
	key->data = get_key(block, i);
	key->size = get_key_size(block, i);
	value->data = get_value(block, i);
	value->size = get_value_size(block, i);
}

void set_data(Block block, int i, struct DBT *key, struct DBT *value)
{
	set_key(block, i, key->data, key->size);
	set_key_size(block, i, key->size);
	set_value(block, i, value->data, value->size);
	set_value_size(block, i, value->size);
}

int cmp(struct DBT *key1, struct DBT *key2)
{
	int size, res;
	if (key1->size <= key2->size)
		size = key1->size;
	else
		size = key2->size;

	res = strncmp(key1->data, key2->data, size);
	if (!res) {
		if (key1->size == key2->size)
			return 0;
		else if (key1->size > key2->size)
			return 1;
		else
			return -1;
	} else
		return res;
}
