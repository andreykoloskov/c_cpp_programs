#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libdb.h"

//Поиск
int db_get(struct DB *db, void *key, size_t key_len, void **val, size_t *val_len)
{
	struct DBT keyt = {
		.data = key,
		.size = key_len
	};
	struct DBT valt = {0, 0};
	int rc = db->get(db, &keyt, &valt);
	*val = valt.data;
	*val_len = valt.size;

	return rc;
}

//Внутренняя реализация поиска
int get(struct DB *db, struct DBT *key, struct DBT *data)
{
	Block block = (Block) malloc(db->head.chunk_size);
	struct DBT key1, value1;
	memcpy(block, db->root, db->head.chunk_size);
	int res = internal_get(db, block, key);
	if (res != -1) {
		get_data(block, res, &key1, &value1);
		data->data = malloc(value1.size);
		data->size = value1.size;
		memcpy((data->data), value1.data, data->size);

		if (block)
			free(block);
		return 0;
	} else {
		if (block)
			free(block);
		return -1;
	}
}

int internal_get(struct DB *db, Block block, struct DBT *key)
{
	int i = 0;
	struct DBT key1, value1;
	while (i < get_n(block)) {
		get_data(block, i, &key1, &value1);
		if (cmp(key, &key1) <= 0)
			break;
		i++;
	}

	if (i < get_n(block) && !cmp(key, &key1))
		return i;

	if (get_leaf(block))
		return -1;
	else {
		int id = get_c(block, i);
		db->block_read(db, block, id);
		return internal_get(db, block, key);
	}

	return -1;
}
