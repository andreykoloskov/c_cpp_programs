#include <string.h>

/* check `man dbopen` */

#define KEY_SIZE 300
#define VALUE_SIZE 300

typedef unsigned char * Block;

//Мета информация базы данных
struct DBC
{
        /* Maximum on-disk file size */
        /* 512MB by default */
	//Максимальный размер файла бд, иначе возвращать ошибку
        size_t db_size;
        /* Maximum chunk (node/data chunk) size */
        /* 4KB by default */
	//Размер блока в b - дереве
        size_t chunk_size;
        /* Maximum memory size */
        /* 16MB by default */
	//Пока не нужен
        //size_t mem_size;
};

//Данные с размером
struct DBT
{
     void  *data;
     size_t size;
};

//Заголовок базы данных
struct Head
{
	size_t db_size;						//Размер файла БД
    size_t chunk_size;					//Размер блока в B дереве
    //size_t mem_size;					//Размер памяти (пока не нужен)
	int stat_offset;					//Смещение начала области статистики (в блоках)
	int stat_count;						//Количество блоков статистики
	int data_offset;					//Смещение начала области данных (в блоках)
	int data_count;						//Количество блоков данных
	int root_id;						//Номер блока для главного элемента
	int t;								//Параметр T B дерева
};

struct NodeHead
{
	int n;								//Фактическое количество ключей
	int leaf;							//1 - если лист, 0 - если внутренний узел
	int num;							//Номер блока в массиве блоков
};

struct NodeData
{
	int key_size;
	unsigned char key[KEY_SIZE];
	int value_size;
	unsigned char value[VALUE_SIZE];
	int c;
};

struct DB {
    /* Public API */
    /* Returns 0 on OK, -1 on Error */
    int (*close)(struct DB *db);
    int (*del)(struct DB *db, struct DBT *key);
    /* * * * * * * * * * * * * *
	* Returns malloc'ed data into 'struct DBT *data'.
	* Caller must free data->data. 'struct DBT *data' must be alloced in
	* caller.
	* * * * * * * * * * * * * */
    int (*get)(struct DB *db, struct DBT *key, struct DBT *data);
    int (*put)(struct DB *db, struct DBT *key, struct DBT *data);
    /* For future uses - sync cached pages with disk
	* int (*sync)(const struct DB *db)
	* */

	/* Private API */
	FILE *fd;							//Файловый дескриптор базы данных
	struct Head head;					//Заголовок базы данных
	Block block_stat;					//Статистика использования блоков базы данных
	Block root;		 					//Головной элемент B дерева базы данных

	void (*block_read)(struct DB *db, Block block, int id);				//Чтение блока
	void (*block_write)(struct DB *db, Block block, int id); 			//Запись блока
	int (*block_alloc)(struct DB *db);	 								//Выделение блока
	void (*block_free)(struct DB *db, int id); 							//Освобождение блока
}; /* Need for supporting multiple backends (HASH/BTREE) */

struct DB *dbcreate(char *file, struct DBC conf);
struct DB *dbopen  (char *file, struct DBC conf); /* Metadata in file */

int db_db_close(struct DB *db);
int db_db_del(struct DB *db, struct DBT *key);
int db_db_get(struct DB *db, struct DBT *key, struct DBT *data);
int db_db_put(struct DB *db, struct DBT *key, struct DBT *data);

void db_block_read(struct DB *db, Block block, int id);
void db_block_write(struct DB *db, Block block, int id);
int db_block_alloc(struct DB *db);
void db_block_free(struct DB *db, int id);
void db_block_free_root(struct DB *db, int id);

int db_may_del(struct DB *db, Block block, struct DBT *key);
void db_may_del_from_leaf_1(struct DB *db, Block x, struct DBT *key);
void db_may_del_from_not_leaf_2a(struct DB *db, Block x, Block y, Block dop, struct DBT *key, int i);
void db_may_del_from_not_leaf_2b(struct DB *db, Block x, Block z, Block dop, struct DBT *key, int i);
void db_may_del_from_not_leaf_2c(struct DB *db, Block x, Block y, Block z, Block dop, struct DBT *key, int i);
void db_may_del_from_not_leaf_3a_l(struct DB *db, Block x, Block c, Block l, struct DBT *key, int i);
void db_may_del_from_not_leaf_3a_r(struct DB *db, Block x, Block c, Block r, struct DBT *key, int i);
void db_may_del_from_not_leaf_3b(struct DB *db, Block x, Block c, Block l, Block r, struct DBT *key, int i);
int db_my_get(struct DB *db, Block block, struct DBT *key);
void b_tree_split_child(struct DB *db, Block x, int i, Block y);
void b_tree_insert_nonfull(struct DB *db, Block x, struct DBT *key, struct DBT *data);

void print_db(struct DB *db);
void print_block(struct DB *db, int id);

int db_close(struct DB *db);
int db_del(struct DB *, void *, size_t);
int db_get(struct DB *, void *, size_t, void **, size_t *);
int db_put(struct DB *, void *, size_t, void * , size_t  );
