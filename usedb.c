#include "libdb.h"

//Создание базы
struct DB *dbcreate(char *file, const struct DBC conf)
{
	//Проверка корректности метаданных
	if (conf.db_size > 512 * 1024 * 1024)
		return NULL;
	if (conf.chunk_size > 4 * 1024)
		return NULL;
	if (conf.chunk_size && conf.db_size % conf.chunk_size)
		return NULL;
	if (conf.mem_size > 16 * 1024 * 1024)
		return NULL;

	unsigned char end_file;
	struct DB *db = (struct DB *) malloc(sizeof(*db));

	//Файловый дескриптор базы данных
	if ((db->fd = fopen(file, "w")) == NULL)
		return NULL;

	//Заголовок базы данных
	//Общая информация
	if (conf.db_size < 1024 * 1024)
		db->head.db_size = 512 * 1024 * 1024;
	else
		db->head.db_size = conf.db_size;

	if (conf.chunk_size < 4 * 1024)
		db->head.chunk_size = 4 * 1024;
	else
		db->head.chunk_size = conf.chunk_size;
	if (conf.mem_size < 4 * 1024)
		db->head.mem_size = 16 * 1024 * 1024;
	else
		db->head.mem_size = conf.mem_size;

	//Смещение области статистики (в блоках)
	db->head.stat_offset = 1;
	//Количество блоков статистики
	int offset = (db->head.db_size / db->head.chunk_size) / 8;
	db->head.stat_count = offset / db->head.chunk_size + 1;
	//Смещение области данных (в блоках)
	db->head.data_offset = db->head.stat_offset + db->head.stat_count;
	//Количество блоков данных (кратно 8)
	offset = db->head.db_size / db->head.chunk_size;
	db->head.data_count = ((offset - db->head.data_offset) / 8) * 8;
	//Номер блока для главного элемента
	db->head.root_id = 0;
	//Параметр T B дерева
	offset = db->head.chunk_size - sizeof(struct NodeHead);
	db->head.t = (offset / (sizeof(struct NodeData))) / 2;
	fwrite(&db->head, sizeof(db->head), 1, db->fd);

	//Статистика использования блоков
	int sz = db->head.stat_count * db->head.chunk_size;
	db->block_stat = (Block) malloc(sz);
	db->block_stat[0] |= (0x1 << 7);
	fseek(db->fd, db->head.stat_offset * db->head.chunk_size, SEEK_SET);
	sz = db->head.stat_count * db->head.chunk_size;
	fwrite(db->block_stat, sz, 1, db->fd);

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

	if (db->fd)
		fclose(db->fd);
	if (db->block_stat)
		free(db->block_stat);
	if (db->root)
		free(db->root);
	if (db)
		free(db);

	//Открытие созданной БД
	db = dbopen(file, conf);

	return db;
}

//Открытие базы
struct DB *dbopen (char *file, const struct DBC conf)
{
	struct DB *db = (struct DB *) malloc(sizeof(*db));
	//Функции
	db->close = close;
	db->del = del;
	db->get = get;
	db->put = put;
	db->block_read = db_block_read;
	db->block_write = db_block_write;
	db->block_alloc = db_block_alloc;
	db->block_free = db_block_free;

	//Файловый дескриптор
	if ((db->fd = fopen(file, "r+")) == NULL)
		return NULL;

	//Заголовок базы данных
	fread(&db->head, sizeof(db->head), 1, db->fd);
	//Статистика использования блоков
	int sz = db->head.stat_count * db->head.chunk_size;
	db->block_stat = (Block) malloc(sz);
	fseek(db->fd, db->head.stat_offset * db->head.chunk_size, SEEK_SET);
	fread(db->block_stat, sz, 1, db->fd);
	//Головной элемент B дерева базы данных
	db->root = (Block) malloc(db->head.chunk_size);
	int offset = db->head.data_offset + db->head.root_id;
	fseek(db->fd, offset * db->head.chunk_size, SEEK_SET);
	fread(db->root, db->head.chunk_size, 1, db->fd);

	//Кэш
	db->cash.cash_read = db_cash_read;
	db->cash.cash_write = db_cash_write;
	db->cash.cash_search = db_cash_search;
	db->cash.cash_insert = db_cash_insert;
	db->cash.cash_delete = db_cash_delete;
	//Статистика кэша
	db->cash.size = db->head.mem_size / db->head.chunk_size;
	db->cash.rt = NULL;
	int i;
	db->cash.use = (int *) malloc(db->cash.size * sizeof(int));
	for (i = 0; i < db->cash.size; i++)
		db->cash.use[i] = 0;

	db->cash.num = (int *) malloc(db->cash.size * sizeof(int));
	for (i = 0; i < db->cash.size; i++)
		db->cash.num[i] = -1;

	db->cash.links = (Age **) malloc(db->cash.size * sizeof(Age *));
	Age *age = (Age *) malloc(sizeof(Age));
	Age *age2;
	age->cash_id = 0;
	age->prev = NULL;
	age->next = NULL;
	db->cash.start = age;
	db->cash.links[0] = age;
	for (i = 1; i < db->cash.size; i++) {
		Age *elem = (Age *) malloc(sizeof(Age));
		elem->cash_id = i;
		elem->prev = age;
		elem->next = NULL;
		age->next = elem;
		age = age->next;
		db->cash.links[i] = age;
	}
	db->cash.finish = age;

	//Массив блоков кэша
	sz = db->cash.size * db->head.chunk_size;
	db->cash.block = (Block) malloc(sz);

	return db;
}

//Закрытие базы
int db_close(struct DB *db)
{
	db->close(db);
}

//Внутренняя реализация закрытия базы
int close(struct DB *db)
{
	if (!db)
		return 0;
	if (db->fd)
		fclose(db->fd);
	if (db->block_stat)
		free(db->block_stat);
	if (db->root)
		free(db->root);
	if (db->cash.block)
		free(db->cash.block);
	if (db->cash.use)
		free(db->cash.use);
	if (db->cash.num)
		free(db->cash.num);

	int i;
	for (i = 0; i < db->cash.size; i++)
		if (db->cash.links[i])
			free(db->cash.links[i]);

	if (db->cash.links)
		free(db->cash.links);
	if (db->cash.rt)
		tfree(db->cash.rt);
	if (db)
		free(db);

	return 0;
}
