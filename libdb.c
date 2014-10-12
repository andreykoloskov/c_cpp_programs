#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libdb.h"


//Функции для облегчения доступа к блоку
inline int get_n(Block block)
{
	return ((struct NodeHead *) block)->n;
}

inline void set_n(Block block, int n)
{
	((struct NodeHead *) block)->n = n;
}
/////////////////////////////////////////
inline int get_leaf(Block block)
{
	return ((struct NodeHead *) block)->leaf;
}

inline void set_leaf(Block block, int leaf)
{
	((struct NodeHead *) block)->leaf = leaf;
}
//////////////////////////////////////////
inline int get_num(Block block)
{
	return ((struct NodeHead *) block)->num;
}

inline void set_num(Block block, int num)
{
	((struct NodeHead *) block)->num = num;
}
///////////////////////////////////////////
inline int get_key_size(Block block, int i)
{
	return (int) ((struct NodeData *) (block + sizeof(struct NodeHead) + i * sizeof(struct NodeData)))->key_size;
}

inline void set_key_size(Block block, int i, int key_size)
{
	((struct NodeData *) (block + sizeof(struct NodeHead) + i * sizeof(struct NodeData)))->key_size = key_size;
}
///////////////////////////////////////////
inline void * get_key(Block block, int i)
{
	/*
	printf("(%d %d, key = ", get_num(block), get_n(block));
	printf("%c", (((struct NodeData *) (block + sizeof(struct NodeHead) + i * sizeof(struct NodeData)))->key)[0]);
	printf("%c", (((struct NodeData *) (block + sizeof(struct NodeHead) + i * sizeof(struct NodeData)))->key)[1]);
	printf("%c) ", (((struct NodeData *) (block + sizeof(struct NodeHead) + i * sizeof(struct NodeData)))->key)[2]);
	*/

	return ((struct NodeData *) (block + sizeof(struct NodeHead) + i * sizeof(struct NodeData)))->key;
}

inline void set_key(Block block, int i, void *key, int key_size)
{
	memcpy(((struct NodeData *) (block + sizeof(struct NodeHead) + i * sizeof(struct NodeData)))->key, key, key_size);
}
///////////////////////////////////////////
inline int get_value_size(Block block, int i)
{
	return (int) ((struct NodeData *) (block + sizeof(struct NodeHead) + i * sizeof(struct NodeData)))->value_size;
}

inline void set_value_size(Block block, int i, int value_size)
{
	((struct NodeData *) (block + sizeof(struct NodeHead) + i * sizeof(struct NodeData)))->value_size = value_size;
}
///////////////////////////////////////////
inline void * get_value(Block block, int i)
{
	return ((struct NodeData *) (block + sizeof(struct NodeHead) + i * sizeof(struct NodeData)))->value;
}

inline void set_value(Block block, int i, void *value, int value_size)
{
	memcpy(((struct NodeData *) (block + sizeof(struct NodeHead) + i * sizeof(struct NodeData)))->value, value, value_size);
}
////////////////////////////////////////////
inline int get_c(Block block, int i)
{
	return (int) ((struct NodeData *) (block + sizeof(struct NodeHead) + i * sizeof(struct NodeData)))->c;
}
inline void set_c(Block block, int i, int c)
{
	((struct NodeData *) (block + sizeof(struct NodeHead) + i * sizeof(struct NodeData)))->c = c;
}
/////////////////////////////////////////////
inline int db_strncmp(void *key_data1, int key_size1, void *key_data2, int key_size2)
{
	int size, res;
	if (key_size1 <= key_size2) size = key_size1;
	else size = key_size2;

	res = strncmp(key_data1, key_data2, size);
	if (!res) {
		if (key_size1 == key_size2) return 0;
		else if (key_size1 > key_size2) return 1;
		else return -1;
	} else return res;
}
/////////////////////////////////////////////

//Создание базы
struct DB *dbcreate(const char *file, const struct DBC conf)
{
	//Проверка корректности метаданных
	if (conf.db_size > 512 * 1024 * 1024) return NULL;
	if (conf.chunk_size > 4 * 1024) return NULL;
	//if (conf->mem_size > 16 * 1024 * 1024) return NULL;
	if (conf.chunk_size && conf.db_size % conf.chunk_size) return NULL;

	unsigned char end_file;

	struct DB *db = (struct DB *) malloc(sizeof(*db));

	//Файловый дескриптор базы данных
	if ((db->fd = fopen(file, "w")) == NULL) return NULL;

	//Заголовок базы данных
	//Общая информация
	if (conf.db_size < 1024 * 1024) db->head.db_size = 512 * 1024 * 1024;
	else db->head.db_size = conf.db_size;

	if (conf.chunk_size < 512) db->head.chunk_size = 4 * 1024;
	else db->head.chunk_size = conf.chunk_size;

	//if (conf->mem_size < 1024 * 1024) db->head.mem_size = 16 * 1024 * 1024;
	//else db->head.mem_size = conf->mem_size;

	//Смещение начала области статистики (в блоках)
	db->head.stat_offset = 1;
	//Количество блоков статистики
	db->head.stat_count = ((db->head.db_size /  db->head.chunk_size) / 8) / db->head.chunk_size + 1;
	//Смещение начала области данных (в блоках)
	db->head.data_offset = db->head.stat_offset + db->head.stat_count;
	//Количество блоков данных (кратно 8, т.к. используем битовый массив для контроля)
	db->head.data_count = ((db->head.db_size / db->head.chunk_size - db->head.data_offset) / 8) * 8;
	//Номер блока для главного элемента
	db->head.root_id = 0;
	//Параметр T B дерева
	db->head.t = ((db->head.chunk_size - sizeof(struct NodeHead)) / (sizeof(struct NodeData))) / 2;
	fwrite(&db->head, sizeof(db->head), 1, db->fd);

	//Статистика использования блоков базы данных
	db->block_stat = (Block) malloc(db->head.stat_count * db->head.chunk_size);
	db->block_stat[0] |= (0x1 << 7);
	fseek(db->fd, db->head.stat_offset * db->head.chunk_size, SEEK_SET);
	fwrite(db->block_stat, db->head.stat_count * db->head.chunk_size, 1, db->fd);

	//Головной элемент B дерева базы данных
	db->root = (Block) malloc(db->head.chunk_size);
	set_n(db->root, 0);
	set_leaf(db->root, 1);
	set_num(db->root, 0);
	fseek(db->fd, db->head.data_offset * db->head.chunk_size, SEEK_SET);
	fwrite(db->root, db->head.chunk_size, 1, db->fd);

	//Установка длины файла
	end_file = 0;
	fseek(db->fd, db->head.db_size - 1, SEEK_SET);
	fwrite(&end_file, sizeof(end_file), 1, db->fd);

	if (db->fd) fclose(db->fd);
	if (db->block_stat) free(db->block_stat);
	if (db->root) free(db->root);
	if (db) free(db);

	//Открытие созданной БД
	db = dbopen(file, conf);

	return db;
}

//Открытие базы
struct DB *dbopen  (const char *file, const struct DBC conf)
{
	struct DB *db = (struct DB *) malloc(sizeof(*db));
	//Функции
	db->close = db_db_close;
	db->del = db_db_del;
	db->get = db_db_get;
	db->put = db_db_put;
	db->block_read = db_block_read;
	db->block_write = db_block_write;
	db->block_alloc = db_block_alloc;
	db->block_free = db_block_free;

	//Файловый дескриптор
	if ((db->fd = fopen(file, "r+")) == NULL) return NULL;

	//Заголовок базы данных
	fread(&db->head, sizeof(db->head), 1, db->fd);
	////Статистика использования блоков базы данных
	db->block_stat = (Block) malloc(db->head.stat_count * db->head.chunk_size);
	fseek(db->fd, db->head.stat_offset * db->head.chunk_size, SEEK_SET);
	fread(db->block_stat, db->head.stat_count * db->head.chunk_size, 1, db->fd);
	//Головной элемент B дерева базы данных
	db->root = (Block) malloc(db->head.chunk_size);
	fseek(db->fd, (db->head.data_offset + db->head.root_id) * db->head.chunk_size, SEEK_SET);
	fread(db->root, db->head.chunk_size, 1, db->fd);

	return db;
}

//Закрытие базы
int db_db_close(struct DB *db)
{
	if (!db) return 0;
	if (db->fd) fclose(db->fd);
	if (db->block_stat) free(db->block_stat);
	if (db->root) free(db->root);
	if (db) free(db);

	return 0;
}

//Удаление
int db_db_del(struct DB *db, struct DBT *key)
{
	return 0;
}

//Поиск
int db_db_get(struct DB *db, struct DBT *key, struct DBT *data)
{
	Block block = (Block) malloc(db->head.chunk_size);
	memcpy(block, db->root, db->head.chunk_size);
	int res = db_my_get(db, block, key);
	if (res != -1) {
		data->size =  get_value_size(block, res);
		data->data = malloc(data->size);
		memcpy((data->data), get_value(block, res), data->size);
		if (block) free(block);
		return 0;
	} else {
		if (block) free(block);
		return -1;
	}
}

//Внутренняя реализация поиска (возвращает порядковый номер в блоке или -1)
int db_my_get(struct DB *db, Block block, struct DBT *key)
{
	int i = 0;
	while(i < get_n(block) && db_strncmp(key->data, key->size, get_key(block, i), get_key_size(block, i)) > 0)
		i++;

	if (i < get_n(block) && !db_strncmp(key->data, key->size, get_key(block, i), get_key_size(block, i)))
		return i;

	if (get_leaf(block))
		return -1;
	else {
		int id = get_c(block, i);
		db->block_read(db, block, id);
		return db_my_get(db, block, key);
	}

	return -1;
}

//Вставка
int db_db_put(struct DB *db, struct DBT *key, struct DBT *data)
{
	Block r = (Block) malloc(db->head.chunk_size);
	memcpy(r, db->root, db->head.chunk_size);
	int res = db_my_get(db, r, key);
    if (res != -1) {
     	set_value_size(r, res, data->size);
    	set_value(r, res, data->data, data->size);
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
		b_tree_insert_nonfull(db, db->root, key, data);
		if (s) free(s);
    } else {
    	b_tree_insert_nonfull(db, r, key, data);
    }
/*
	printf("(n = %d, num = %d, ", get_n(r), get_num(r));
    printf("%c", ((unsigned char *) (key->data))[0]);
	printf("%c", ((unsigned char *) (key->data))[1]);
	printf("%c) ", ((unsigned char *) (key->data))[2]);
*/
	if (r) free(r);

	return 0;
}

void b_tree_split_child(struct DB *db, Block x, int i, Block y)
{
	int j;
	int t = db->head.t;
	int id = db_block_alloc(db);
	Block z = (Block) malloc(db->head.chunk_size);
	set_leaf(z, get_leaf(y));
	set_n(z, t - 1);
	set_num(z, id);
	for (j = 0; j < t - 1; j++){
		set_key_size(z, j, get_key_size(y, j + t));
		set_key(z, j, get_key(y, j + t), get_key_size(y, j + t));
		set_value_size(z, j, get_value_size(y, j + t));
		set_value(z, j, get_value(y, j + t), get_value_size(y, j + t));
	}

	if (!get_leaf(y))
		for (j = 0; j < t; j++)
			set_c(z, j, get_c(y, j + t));

	set_n(y, t - 1);
	for (j = get_n(x); j >= i; j--) {
		set_c(x, j + 1, get_c(x, j));
	}

	set_c(x, i + 1, get_num(z));

	for (j = get_n(x) - 1; j >=0, j >= i - 1; j--) {
		set_key_size(x, j + 1, get_key_size(x, j));
		set_key(x, j + 1, get_key(x, j), get_key_size(x, j));
		set_value_size(x, j + 1, get_value_size(x, j));
		set_value(x, j + 1, get_value(x, j), get_value_size(x, j));
	}
	set_key_size(x, i, get_key_size(y, t - 1));
	set_key(x, i, get_key(y, t - 1), get_key_size(y, t - 1));
	set_value_size(x, i, get_value_size(y, t - 1));
	set_value(x, i, get_value(y, t - 1), get_value_size(y, t - 1));

	set_n(x, get_n(x) + 1);

	db_block_write(db, y, get_num(y));
	db_block_write(db, z, get_num(z));
	db_block_write(db, x, get_num(x));
	if (z) free(z);
}

void b_tree_insert_nonfull(struct DB *db, Block x, struct DBT *key, struct DBT *data)
{
	int i = get_n(x);
	int t = db->head.t;
	int id;
	if (get_leaf(x)) {
		while (i > 0 && db_strncmp(key->data, key->size, get_key(x, i - 1), get_key_size(x, i - 1)) < 0) {
			set_key_size(x, i, get_key_size(x, i - 1));
			set_key(x, i, get_key(x, i - 1), get_key_size(x, i - 1));
			set_value_size(x, i, get_value_size(x, i - 1));
			set_value(x, i, get_value(x, i - 1), get_value_size(x, i - 1));
			i--;
		}
		set_key_size(x, i, key->size);
		set_key(x, i, key->data, key->size);
		set_value_size(x, i, data->size);
		set_value(x, i, data->data, data->size);
		set_n(x, get_n(x) + 1);
		db_block_write(db, x, get_num(x));
		if (get_num(x) == get_num(db->root))
			db_block_read(db, db->root, get_num(x));
	} else {
		while (i > 0 && db_strncmp(key->data, key->size, get_key(x, i - 1), get_key_size(x, i - 1)) < 0)
			i--;
		Block block = (Block) malloc(db->head.chunk_size);
		db_block_read(db, block, get_c(x, i));
		if (get_n(block) == 2 * t - 1) {
			b_tree_split_child(db, x, i, block);
			if (db_strncmp(key->data, key->size, get_key(x, i),  get_key_size(x, i)) > 0)
				i++;
		}
		b_tree_insert_nonfull(db, block, key, data);
		if (block) free(block);
	}
}


void db_block_read(struct DB *db, Block block, int id)
{
	fseek(db->fd, (db->head.data_offset + id) * db->head.chunk_size, SEEK_SET);
	fread(block, db->head.chunk_size, 1, db->fd);
}

void db_block_write(struct DB *db, Block block, int id)
{
	fseek(db->fd, (db->head.data_offset + id) * db->head.chunk_size, SEEK_SET);
	fwrite(block, db->head.chunk_size, 1, db->fd);
	if (db->head.root_id == get_num(block)) {
		memcpy(db->root, block, db->head.chunk_size);
		fseek(db->fd, (db->head.data_offset + db->head.root_id) * db->head.chunk_size, SEEK_SET);
		fwrite(db->root, db->head.chunk_size, 1, db->fd);
		fseek(db->fd, 0, SEEK_SET);
		fwrite(&db->head, sizeof(db->head), 1, db->fd);
	}
}

int db_block_alloc(struct DB *db)
{
	int i, j;
	unsigned char a;
	for (i = 0; i < db->head.data_count / 8, db->block_stat[i] == 0xff; ++i);

	if (i == db->head.data_count / 8)
		return -1;
	else {
		for (j = 0; j < 8; ++j) {
			if (!((db->block_stat[i]) & (0x1 << (7 - j)))) {
				(db->block_stat[i]) |= (0x1 << (7 - j));
				break;
			}
		}
	}

	int id = 8 * i + j;
	fseek(db->fd, db->head.stat_offset * db->head.chunk_size, SEEK_SET);
	fwrite(db->block_stat, db->head.stat_count * db->head.chunk_size, 1, db->fd);

	return id;
}

void db_block_free(struct DB *db, int id)
{
	db->block_stat[id / 8] &= ~((0x1 << 7) >> (id % 8));
	fseek(db->fd, db->head.stat_offset * db->head.chunk_size, SEEK_SET);
	fwrite(db->block_stat, db->head.stat_count * db->head.chunk_size, 1, db->fd);
}


void print_db(struct DB *db)
{
	int i, j;
	printf("db_size = %d, chunk_size = %d,\n", db->head.db_size, db->head.chunk_size);
	printf("stat_offset = %d, stat_count = %d, data_offset = %d, data_count = %d,\n", db->head.stat_offset, db->head.stat_count, db->head.data_offset, db->head.data_count);
	printf("root_id = %d, head.t = %d, root_n = %d, root_leaf = %d, root_num = %d\n\n", db->head.root_id, db->head.t, ((struct NodeHead *)db->root)->n, ((struct NodeHead *)db->root)->leaf, ((struct NodeHead *)db->root)->num);

	for (i = 0; i < db->head.data_count / 8; ++i)
		if ((db->block_stat)[i])
			for (j = 0; j < 8; ++j)
				if ((db->block_stat[i]) & (0x1 << (7 - j))) print_block(db, i * 8 + j);
}

void print_block(struct DB *db, int id)
{
	int i, j;
	Block block = (Block) malloc(db->head.chunk_size);
	db_block_read(db, block, id);
	printf("id = %d; n = %d; leaf = %d; num = %d\n", id, get_n(block), get_leaf(block), get_num(block));
	unsigned char *key, *value;
	int key_size, value_size;
	for (i = 0; i < get_n(block); ++i) {
		key_size = get_key_size(block, i);
		key = get_key(block, i);
		value_size = get_value_size(block, i);
		value = get_value(block, i);
		printf("(key = ");
		for (j = 0; j < key_size; j++)
			printf("%c", key[j]);
		printf(", value = ");
		for (j = 0; j < value_size; j++)
			printf("%c", value[j]);
		printf(", cl = %d, cr = %d) ", get_c(block, i), get_c(block, i + 1));
	}
	printf("\n\n");
	if (block) free(block);
}

///////////////////////////////////////////////////////////////////////////////////////

int db_close(struct DB *db)
{
	db->close(db);
}

int db_del(struct DB *db, void *key, size_t key_len)
{
	struct DBT keyt = {
		.data = key,
		.size = key_len
	};
	return db->del(db, &keyt);
}

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
