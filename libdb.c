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
inline void set_key_value(Block block, int i, void *key_data, int key_size, void *value_data, int value_size)
{
	set_key_size(block, i, key_size);
	set_key(block, i, key_data, key_size);
	set_value_size(block, i, value_size);
	set_value(block, i, value_data, value_size);
}
/////////////////////////////////////////////

//Создание базы
struct DB *dbcreate(char *file, const struct DBC conf)
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
struct DB *dbopen  (char *file, const struct DBC conf)
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
	//Статистика использования блоков базы данных
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
	return db_may_del(db, db->root, key);
}

//1. Если: x - лист, k принадлежит x => удаляем k
// В x всегда будет не менее T элементов (благодаря п. 3)
void db_may_del_from_leaf_1(struct DB *db, Block x, struct DBT *key)
{
	int i, j;
	for (i = 0; i < get_n(x); i++) {
		if (!db_strncmp(key->data, key->size, get_key(x, i), get_key_size(x, i))) {
			for (j = i + 1; j < get_n(x); j++)
				set_key_value(x, j - 1, get_key(x, j), get_key_size(x, j), get_value(x, j), get_value_size(x, j));
			set_n(x, get_n(x) - 1);
			db_block_write(db, x, get_num(x));
		}
	}
	//Если элементов в базе нет
	if (!get_n(x))
		db_block_free_root(db, get_num(x));
}

//2. Если: x - внутренний узел, k принадлежит x =>
//  a) Если: y - дочерний к x, непосредственно предшествует k, y - содержит не менее T ключей =>
//      - Находим k' - предшественник k в поддереве с корнем y;
//      - Рекурсивно удаляем k';
//      - Заменяем k в x ключем k';
void db_may_del_from_not_leaf_2a(struct DB *db, Block x, Block y, Block dop, struct DBT *key, int i)
{
	memcpy(dop, y, db->head.chunk_size);
	while (!get_leaf(dop))
		db_block_read(db, dop, get_c(dop, get_n(dop)));
	struct DBT key1, value1;
	key1.data = get_key(dop, get_n(dop) - 1);
	key1.size = get_key_size(dop, get_n(dop) - 1);
	value1.data = get_value(dop, get_n(dop) - 1);
	value1.size = get_value_size(dop, get_n(dop) - 1);
	db_may_del(db, y, &key1);
	set_key_value(x, i, key1.data, key1.size, value1.data, value1.size);
	db_block_write(db, x, get_num(x));
}

//2. Если: x - внутренний узел, k принадлежит x =>
//  b) Если: z - дочерний к x, непосредственно следует за k, z - содержит не менее T ключей =>
//      - Находим k' - следующий за k в поддереве с корнем z;
//      - Рекурсивно удаляем k';
//      - Заменяем k в x ключем k';
void db_may_del_from_not_leaf_2b(struct DB *db, Block x, Block z, Block dop, struct DBT *key, int i)
{
	memcpy(dop, z, db->head.chunk_size);
	while (!get_leaf(dop))
		db_block_read(db, dop, get_c(dop, 0));
	struct DBT key1, value1;
	key1.data = get_key(dop, 0);
	key1.size = get_key_size(dop, 0);
	value1.data = get_value(dop, 0);
	value1.size = get_value_size(dop, 0);
	set_key_value(x, i, key1.data, key1.size, value1.data, value1.size);
	db_block_write(db, x, get_num(x));
	db_may_del(db, z, &key1);
}

//2. Если: x - внутренний узел, k принадлежит x =>
//  c) Если: y и z - содержат по T - 1 ключей =>
//      - В y вносим k и все ключи из z;
//      - Из x удаляем k и указатель на z;
//      - Если x - корень, и в нем не остается ключей =>
//          -- y - новый корень;
//      - Освобождаем z;
//      - Рекурсиво удаляем k из y;
void db_may_del_from_not_leaf_2c(struct DB *db, Block x, Block y, Block z, Block dop, struct DBT *key, int i)
{
	int j;
	struct DBT value = {0, 0};
	db_db_get(db, key, &value);
	set_n(y, get_n(y) + 1);
	set_key_value(y, get_n(y) - 1, key->data, key->size, value.data, value.size);
	for (j = get_n(y); j < get_n(y) + get_n(z); j++)
		set_key_value(y, j, get_key(z, j - get_n(y)), get_key_size(z, j - get_n(y)), get_value(z, j - get_n(y)), get_value_size(z, j - get_n(y)));
	for (j = get_n(y); j <= get_n(y) + get_n(z); j++)
		set_c(y, j, get_c(z, j - get_n(y)));
	set_n(y, get_n(y) + get_n(z));
	set_n(z, 0);
	for (j = i; j < get_n(x) - 1; j++)
		set_key_value(x, j, get_key(x, j + 1), get_key_size(x, j + 1), get_value(x, j + 1), get_value_size(x, j + 1));
	for (j = i + 1; j < get_n(x); j++)
		set_c(x, j, get_c(x, j + 1));
	set_n(x, get_n(x) - 1);

	if (!get_n(x)) {
		//Получился нулевой корень
		//Это может быть только корень, т.к. в ином случае 0 не получится
		db_block_free(db, db->head.root_id);
		db->head.root_id = get_num(y);
		db_block_write(db, y, get_num(y));
	} else {
		db_block_write(db, x, get_num(x));
		db_block_write(db, y, get_num(y));
	}
	db_block_write(db, z, get_num(z));
	db_block_free(db, get_num(z));
	db_may_del(db, y, key);
}

//3. Если: x - внутренний узел, k - отсутствует в x =>
//      - Находим ci[x] - корень поддерева, которое должно содержать k (если он вообще есть)
//      a) Если: ci[x] - содержит T - 1 ключей, левый сосед из x содержит не менее T ключей =>
//          -- В ci[x] передадим ключ - разделитель между ci[x] и этим соседом из x;
//          -- На место этого ключа переместим крайний ключ из соседнего узла;
//          -- Перенесем указатель из соседнего узла в ci[x];
void db_may_del_from_not_leaf_3a_l(struct DB *db, Block x, Block c, Block l, struct DBT *key, int i)
{
	int j;
	set_n(c, get_n(c) + 1);
	for (j = get_n(c) - 1; j > 0; j--)
		set_key_value(c, j, get_key(c, j - 1), get_key_size(c, j - 1), get_value(c, j - 1), get_value_size(c, j - 1));
	for (j = get_n(c); j > 0; j--)
		set_c(c, j, get_c(c, j - 1));
	set_key_value(c, 0, get_key(x, i - 1), get_key_size(x, i - 1), get_value(x, i - 1), get_value_size(x, i - 1));
	set_c(c, 0, get_c(l, get_n(l)));
	set_key_value(x, i - 1, get_key(l, get_n(l) - 1), get_key_size(l, get_n(l) - 1), get_value(l, get_n(l) - 1), get_value_size(l, get_n(l) - 1));
	set_n(l, get_n(l) - 1);
	db_block_write(db, l, get_num(l));
	db_block_write(db, x, get_num(x));
	db_block_write(db, c, get_num(c));
}

//3. Если: x - внутренний узел, k - отсутствует в x =>
//      - Находим ci[x] - корень поддерева, которое должно содержать k (если он вообще есть)
//      a) Если: ci[x] - содержит T - 1 ключей, правый сосед из x содержит не менее T ключей =>
//          -- В ci[x] передадим ключ - разделитель между ci[x] и этим соседом из x;
//          -- На место этого ключа переместим крайний ключ из соседнего узла;
//          -- Перенесем указатель из соседнего узла в ci[x];
void db_may_del_from_not_leaf_3a_r(struct DB *db, Block x, Block c, Block r, struct DBT *key, int i)
{
	int j;
	set_n(c, get_n(c) + 1);
	set_key_value(c, get_n(c) - 1, get_key(x, i), get_key_size(x, i), get_value(x, i), get_value_size(x, i));
	set_c(c, get_n(c), get_c(r, 0));
	set_key_value(x, i, get_key(r, 0), get_key_size(r, 0), get_value(r, 0), get_value_size(r, 0));
	for (j = 1; j < get_n(r); j++)
		set_key_value(r, j - 1, get_key(r, j), get_key_size(r, j), get_value(r, j), get_value_size(r, j));
	for (j = 1; j <= get_n(r); j++)
		set_c(r, j - 1, get_c(r, j));
	set_n(r, get_n(r) - 1);
	db_block_write(db, r, get_num(r));
	db_block_write(db, x, get_num(x));
	db_block_write(db, c, get_num(c));
}

//3. Если: x - внутренний узел, k - отсутствует в x =>
//      b) Если: ci[x] и оба соседа из x содержат по T - 1 ключей =>
//          -- В ci[x] добавим ключ - разделитель из x и одного (например левого) соседа;
//          -- Если x - корень, и в нем не остается ключей =>
//              --- ci[x] - новый корень;
void db_may_del_from_not_leaf_3b(struct DB *db, Block x, Block c, Block l, Block r, struct DBT *key, int i)
{
	int j;
	    int t = db->head.t;
	if (i > 0 && get_n(l) < t) {
		set_n(c, get_n(c) + get_n(l) + 1);
		for (j = get_n(l) + 1; j < get_n(c); j++)
			set_key_value(c, j, get_key(c, j - get_n(l) - 1), get_key_size(c, j - get_n(l) - 1), get_value(c, j - get_n(l) - 1), get_value_size(c, j - get_n(l) - 1));
		for (j = get_n(l) + 1; j <= get_n(c); j++)
			set_c(c, j, get_c(c, j - get_n(l) - 1));
		for (j = 0; j < get_n(l); j++)
			set_key_value(c, j, get_key(l, j), get_key_size(l, j), get_value(l, j), get_value_size(l, j));
		for (j = 0; j <= get_n(l); j++)
			set_c(c, j, get_c(l, j));
		set_key_value(c, get_n(l), get_key(x, i - 1), get_key_size(x, i - 1), get_value(x, i - 1), get_value_size(x, i - 1));
		set_n(l, 0);
		for (j = i - 1; j < get_n(x) - 1; j++)
			set_key_value(x, j, get_key(x, j + 1), get_key_size(x, j + 1), get_value(x, j + 1), get_value_size(x, j + 1));
		for (j = i - 1; j < get_n(x); j++)
			set_c(x, j, get_c(x, j + 1));
		set_n(x, get_n(x) - 1);
		db_block_write(db, l, get_num(l));
		db_block_free(db, get_num(l));
	} else if (i < get_n(x) && get_n(r) < t) {
		set_key_value(c, get_n(c), get_key(x, i), get_key_size(x, i), get_value(x, i), get_value_size(x, i));
		set_n(c, get_n(c) + 1);
		for (j = get_n(c); j < get_n(c) + get_n(r); j++)
			set_key_value(c, j, get_key(r, j - get_n(c)), get_key_size(r, j - get_n(c)), get_value(r, j - get_n(c)), get_value_size(r, j - get_n(c)));
		for (j = get_n(c); j <= get_n(c) + get_n(r); j++)
			set_c(c, j, get_c(r, j - get_n(c)));
		set_n(c, get_n(c) + get_n(r));
		set_n(r, 0);
		for (j = i; j < get_n(x) - 1; j++)
			set_key_value(x, j, get_key(x, j + 1), get_key_size(x, j + 1), get_value(x, j + 1), get_value_size(x, j + 1));
		for (j = i + 1; j < get_n(x); j++)
			set_c(x, j, get_c(x, j + 1));
		set_n(x, get_n(x) - 1);
		db_block_write(db, r, get_num(r));
		db_block_free(db, get_num(r));
	}
	if (!get_n(x)) {
		//Получился нулевой корень
		//Это может быть только корень, т.к. в ином случае 0 не получится
		db_block_free(db, db->head.root_id);
		db->head.root_id = get_num(c);
		db_block_write(db, c, get_num(c));
	} else {
		db_block_write(db, x, get_num(x));
		db_block_write(db, c, get_num(c));
	}
}


//Внутренняя реализация удаления
int db_may_del(struct DB *db, Block block, struct DBT *key)
{
	Block x = (Block) malloc(db->head.chunk_size);
	memcpy(x, block, db->head.chunk_size);
	int i, t = db->head.t;
    if (get_leaf(x)) db_may_del_from_leaf_1(db, x, key);  /*1*/
    else {
		for (i = 0; i < get_n(x); i++) {
			if (!db_strncmp(key->data, key->size, get_key(x, i), get_key_size(x, i))) {
				Block y = (Block) malloc(db->head.chunk_size), z = (Block) malloc(db->head.chunk_size), dop = (Block) malloc(db->head.chunk_size);
				db_block_read(db, y, get_c(x, i));
				db_block_read(db, z, get_c(x, i + 1));
				if (get_n(y) >= t) db_may_del_from_not_leaf_2a(db, x, y, dop, key, i);  /*2a*/
				else if (get_n(z) >= t) db_may_del_from_not_leaf_2b(db, x, z, dop, key, i);  /*2b*/
				else db_may_del_from_not_leaf_2c(db, x, y, z, dop, key, i);  /*2c*/

				if (y) { free(y); y = NULL; }
                if (z) { free(z); z = NULL; }
                if (dop) { free(dop); dop = NULL; }
                if (x) { free(x); x = NULL; }

                return 0;
			}
		}
		i = 0;
		while (i < get_n(x) && db_strncmp(key->data, key->size, get_key(x, i), get_key_size(x, i)) > 0) i++;
		Block c = (Block) malloc(db->head.chunk_size), l = (Block) malloc(db->head.chunk_size), r = (Block) malloc(db->head.chunk_size);
		db_block_read(db, c, get_c(x, i));
		if (i > 0) db_block_read(db, l, get_c(x, i - 1));
		if (i < get_n(x)) db_block_read(db, r, get_c(x, i + 1));
		if (get_n(c) <= t - 1) {
			if (i > 0 && get_n(l) >= t) db_may_del_from_not_leaf_3a_l(db, x, c, l, key, i);   /*3a_l*/
			else if (i < get_n(x) && get_n(r) >= t) db_may_del_from_not_leaf_3a_r(db, x, c, r, key, i);   /*3a_r*/
			else db_may_del_from_not_leaf_3b(db, x, c, l, r, key, i);   /*3b*/
		}
	    db_may_del(db, c, key);  //      - Рекурсивно удаляем k из поддерева с корем ci[x];
        if (c) { free(c); c = NULL; }
        if (l) { free(l); l = NULL; }
        if (r) { free(r); r = NULL; }
        if (x) { free(x); x = NULL; }
    }
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
	for (j = 0; j < t - 1; j++)
		set_key_value(z, j, get_key(y, j + t), get_key_size(y, j + t), get_value(y, j + t), get_value_size(y, j + t));
	if (!get_leaf(y))
		for (j = 0; j < t; j++)
			set_c(z, j, get_c(y, j + t));

	set_n(y, t - 1);
	for (j = get_n(x); j >= i; j--) {
		set_c(x, j + 1, get_c(x, j));
	}

	set_c(x, i + 1, get_num(z));

	for (j = get_n(x) - 1; (j >= 0) && (j >= i - 1); j--)
		set_key_value(x, j + 1, get_key(x, j), get_key_size(x, j), get_value(x, j), get_value_size(x, j));
	set_key_value(x, i, get_key(y, t - 1), get_key_size(y, t - 1), get_value(y, t - 1), get_value_size(y, t - 1));
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
		db_block_read(db, block, get_c(x, i));
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
	for (i = 0; (i < db->head.data_count / 8) && (db->block_stat[i] == 0xff); ++i);

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

void db_block_free_root(struct DB *db, int id)
{
	db_block_free(db, id);
	if (db->head.root_id == id) {
		set_n(db->root, 0);
		set_leaf(db->root, 1);
		set_num(db->root, 0);
		db_block_write(db, db->root, 0);
		int id = db_block_alloc(db);
		db->head.root_id = id;
	}
}

void print_db(struct DB *db)
{
	int i, j;
	printf("db_size = %d, chunk_size = %d,\n", (int) db->head.db_size, (int) db->head.chunk_size);
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
		//printf(", key_size = %d ", get_key_size(block, i));
		printf(", value = ");
		for (j = 0; j < value_size; j++)
			printf("%c", value[j]);
		//printf(", value_size = %d ", get_value_size(block, i));
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
