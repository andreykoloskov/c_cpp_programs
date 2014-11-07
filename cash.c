#include "libdb.h"

#define MAX 1000000000

//Увеличение возраста кэша
inline void cash_age(struct DB *db)
{
	int i;
	for (i = 0; i < db->cash.size; i++)
		if (db->cash.cash_elements[i].used) {
			int c = db->cash.cash_elements[i].age;
			c %= MAX;
			c++;
			db->cash.cash_elements[i].age = c;
		}
}

//Вытеснение из кэша
inline int cash_block_free(struct DB *db)
{
	int i;
	int max = -1;
	int max_i = -1;
	for (i = 0; i < db->cash.size; i++) {
		if (db->cash.cash_elements[i].used) {
			if (db->cash.cash_elements[i].age > max) {
				max = db->cash.cash_elements[i].age;
				max_i = i;
			}
		} else
			break;
	}
	if (i < db->cash.size)
		return i;
	else {
		db->cash.cash_elements[max_i].used = 0;
		return max_i;
	}
}

//Поиск блока в кэше
int db_cash_search(struct DB *db, int id)
{
	int i;
	for (i = 0; i < db->cash.size; i++)
		if (db->cash.cash_elements[i].used)
			if (db->cash.cash_elements[i].num == id)
				return i;
	return -1;
}

//Удаление блока из кэша
void db_cash_delete(struct DB *db, int id)
{
	int cash_id;
	if ((cash_id = db->cash.cash_search(db, id)) != -1)
		db->cash.cash_elements[cash_id].used = 0;
}

//Чтение блока кэша
void db_cash_read(struct DB  *db, Block block, int id)
{
	int cash_id;
	Block bl;
	if ((cash_id = db->cash.cash_search(db, id)) != -1) {
		bl = db->cash.block + cash_id * db->head.chunk_size;
	} else {
		cash_id = cash_block_free(db);
		int offset = db->head.data_offset + id;
		fseek(db->fd, offset * db->head.chunk_size, SEEK_SET);
		bl = db->cash.block + cash_id * db->head.chunk_size;
		fread(bl, db->head.chunk_size, 1, db->fd);
		db->cash.cash_elements[cash_id].used = 1;
		db->cash.cash_elements[cash_id].num = id;
	}
	memcpy(block, bl, db->head.chunk_size);
	cash_age(db);
	db->cash.cash_elements[cash_id].age = 0;
}

//Запись блока кэша
void db_cash_write(struct DB *db, Block block, int id)
{
	int cash_id;
	Block bl;
	if (( cash_id = db->cash.cash_search(db, id)) != -1) {
		bl = db->cash.block + cash_id * db->head.chunk_size;
		memcpy(bl, block, db->head.chunk_size);
	}

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
