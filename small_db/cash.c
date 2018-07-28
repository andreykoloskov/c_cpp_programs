#include "libdb.h"

//Старение кэша
inline void cash_young(struct DB *db, int cash_id)
{
	Age *elem = db->cash.links[cash_id];
	if (elem->prev == NULL)
		return;

	if (elem->next == NULL) {
		db->cash.finish = elem->prev;
		elem->prev->next = NULL;
	} else {
		elem->prev->next = elem->next;
		elem->next->prev = elem->prev;
	}

	elem->prev = NULL;
	elem->next = db->cash.start;
	elem->next->prev = elem;
	db->cash.start = elem;
}

//Поиск блока в кэше
int db_cash_search(struct DB *db, int id, Node *nd)
{
	nd = find(db->cash.rt, id);
	if (nd)
		return nd->data;
	else
		return -1;
}

//Удаление блока из кэша
void db_cash_delete(struct DB *db, int id)
{
        Node *t = find(db->cash.rt, id);
        if (t) {
		db->cash.use[t->data] = 0;
		db->cash.num[t->data] = -1;
		db->cash.rt = remove_tree(db->cash.rt, id);
	}
}

//Добавление блока в кэш
Node *db_cash_insert(struct DB *db, int key, int data)
{
	Node *t = find(db->cash.rt, key);
	if (!t) {
		db->cash.use[data] = 1;
		db->cash.num[data] = key;
		db->cash.rt = insert(db->cash.rt, key, data);
	}
}

//Чтение блока из кэша
void db_cash_read(struct DB  *db, Block block, int id)
{
	int cash_id, old_id;
	Block bl;
	Node *nd = NULL;
	if ((cash_id = db->cash.cash_search(db, id, nd)) != -1) {
		bl = db->cash.block + cash_id * db->head.chunk_size;
	} else {
		cash_id = db->cash.finish->cash_id;
		bl = db->cash.block + cash_id * db->head.chunk_size;
		if (db->cash.use[cash_id]) {
			old_id = db->cash.num[cash_id];
			db->cash.cash_delete(db, old_id);
		}

		int offset = db->head.data_offset + id;
		fseek(db->fd, offset * db->head.chunk_size, SEEK_SET);
		bl = db->cash.block + cash_id * db->head.chunk_size;
		fread(bl, db->head.chunk_size, 1, db->fd);
		db->cash.cash_insert(db, id, cash_id);
	}
	cash_young(db, cash_id);
	memcpy(block, bl, db->head.chunk_size);
}

//Запись блока в кэш
void db_cash_write(struct DB *db, Block block, int id)
{
	int cash_id, old_id;
	Block bl;
	Node *nd = NULL;
	if (( cash_id = db->cash.cash_search(db, id, nd)) != -1) {
		bl = db->cash.block + cash_id * db->head.chunk_size;
		memcpy(bl, block, db->head.chunk_size);
	} else {
		cash_id = db->cash.finish->cash_id;
		bl = db->cash.block + cash_id * db->head.chunk_size;
		if (db->cash.use[cash_id]) {
			old_id = db->cash.num[cash_id];
			db->cash.cash_delete(db, old_id);
		}
		memcpy(bl, block, db->head.chunk_size);
		db->cash.cash_insert(db, id, cash_id);
	}
	cash_young(db, cash_id);
	int offset = db->head.data_offset + id;
	fseek(db->fd, offset * db->head.chunk_size, SEEK_SET);
	fwrite(block, db->head.chunk_size, 1, db->fd);
	if (db->head.root_id == get_num(block)) {
		int offset2 = db->head.data_offset + db->head.root_id;
		memcpy(db->root, block, db->head.chunk_size);
		int offset = offset2 * db->head.chunk_size;
		fseek(db->fd, offset, SEEK_SET);
		fwrite(db->root, db->head.chunk_size, 1, db->fd);
		fseek(db->fd, 0, SEEK_SET);
		fwrite(&db->head, sizeof(db->head), 1, db->fd);
	}
}

//Печать доступности кэша
void print_use(struct DB *db)
{
	int i;
	for (i = 0; i < db->cash.size; i++)
		printf("(%d, %d)", i, db->cash.use[i]);
}

//Печать кэша
void print_cash(struct DB *db)
{
	Age *i = db->cash.start;
	int k = 0;
	while (1) {
		printf("(%d, %d)", k, i->cash_id);
		if (i == db->cash.finish)
			break;
		i = i->next;
		k++;
	}
}
