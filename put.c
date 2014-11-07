#include "libdb.h"

//Вставка
int db_put(struct DB *db, void *key, size_t key_len, void *val, size_t val_len)
{
	struct DBT keyt = {
		.data = key,
		.size = key_len
	};
	struct DBT valt = {
		.data = val,
		.size = val_len
	};
	return db->put(db, &keyt, &valt);
}

//Внутренняя реализация вставки
int put(struct DB *db, struct DBT *key, struct DBT *data)
{
	Block r = (Block) malloc(db->head.chunk_size);
	memcpy(r, db->root, db->head.chunk_size);
	int res = internal_get(db, r, key);
	if (res != -1) {
		set_data(r, res, key, data);
		db_block_write(db, r, get_num(r));
		if (r) free(r);
			return 0;
	}
	memcpy(r, db->root, db->head.chunk_size);
	if (get_n(r) == 2 * db->head.t - 1) {
		int id = db_block_alloc(db);
		Block s = (Block) malloc(db->head.chunk_size);
		set_n(s, 0);
		set_leaf(s, 0);
		set_num(s, id);
		set_c(s, 0, db->head.root_id);
		memcpy(db->root, s, db->head.chunk_size);
		db->head.root_id = id;
		b_tree_split_child(db, db->root, 0, r);
		b_tree_ins_nf(db, db->root, key, data);
		if (s)
			free(s);
	} else {
		b_tree_ins_nf(db, r, key, data);
	}

	if (r)
		free(r);

	return 0;
}

void b_tree_split_child(struct DB *db, Block x, int i, Block y)
{
	int j;
	int t = db->head.t;
	int id = db_block_alloc(db);
	Block z = (Block) malloc(db->head.chunk_size);
	struct DBT key, value;
	set_leaf(z, get_leaf(y));
	set_n(z, t - 1);
	set_num(z, id);
	for (j = 0; j < t - 1; j++) {
		get_data(y, j + t, &key, &value);
		set_data(z, j, &key, &value);
	}
	if (!get_leaf(y))
		for (j = 0; j < t; j++)
			set_c(z, j, get_c(y, j + t));

	set_n(y, t - 1);
	for (j = get_n(x); j >= i; j--) {
		set_c(x, j + 1, get_c(x, j));
	}

	set_c(x, i + 1, get_num(z));

	for (j = get_n(x) - 1; (j >= 0) && (j >= i - 1); j--) {
		get_data(x, j, &key, &value);
		set_data(x, j + 1, &key, &value);
	}
	get_data(y, t - 1, &key, &value);
	set_data(x, i, &key, &value);

	set_n(x, get_n(x) + 1);
	db_block_write(db, y, get_num(y));
	db_block_write(db, z, get_num(z));
	db_block_write(db, x, get_num(x));
	if (z)
		free(z);
}

void b_tree_ins_nf(struct DB *db, Block x, struct DBT *key, struct DBT *data)
{
	int i = get_n(x);
	int t = db->head.t;
	struct DBT key1, value1;
	if (get_leaf(x)) {
		while (i > 0) {
			get_data(x, i - 1, &key1, &value1);
			if (cmp(key, &key1) >= 0)
				break;
			set_data(x, i, &key1, &value1);
			i--;
		}
		set_data(x, i, key, data);
		set_n(x, get_n(x) + 1);

		db_block_write(db, x, get_num(x));
		if (get_num(x) == get_num(db->root))
			db_block_read(db, db->root, get_num(x));
	} else {
		while (i > 0) {
			get_data(x, i - 1, &key1, &value1);
			if (cmp(key, &key1) >= 0)
				break;
			i--;
		}
		Block block = (Block) malloc(db->head.chunk_size);
		db_block_read(db, block, get_c(x, i));
		if (get_n(block) == 2 * t - 1) {
			b_tree_split_child(db, x, i, block);
			get_data(x, i, &key1, &value1);
			if (cmp(key, &key1) > 0)
				i++;
		}
		db_block_read(db, block, get_c(x, i));
		b_tree_ins_nf(db, block, key, data);
		if (block) free(block);
	}
}
