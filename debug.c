#include "libdb.h"

void print_db(struct DB *db)
{
	int i, j;
	printf("db_size = %d, ", (int) db->head.db_size);
	printf("chunk_size = %d,\n", (int) db->head.chunk_size);
	printf("stat_offset = %d, ", db->head.stat_offset);
	printf("stat_count = %d, ", db->head.stat_count);
	printf("data_offset = %d, ", db->head.data_offset);
	printf("data_count = %d,\n", db->head.data_count);
	printf("root_id = %d, ", db->head.root_id);
	printf("head.t = %d, ", db->head.t);
	printf("root_n = %d, ", ((struct NodeHead *)db->root)->n);
	printf("root_leaf = %d, ", ((struct NodeHead *)db->root)->leaf);
	printf("root_num = %d\n\n", ((struct NodeHead *)db->root)->num);

	for (i = 0; i < db->head.data_count / 8; i++)
		if ((db->block_stat)[i])
			for (j = 0; j < 8; ++j)
				if ((db->block_stat[i]) & (0x1 << (7 - j)))
					print_block(db, i * 8 + j);
}

void print_block(struct DB *db, int id)
{
	int i, j;
	struct DBT key, value;
	Block block = (Block) malloc(db->head.chunk_size);
	db_block_read(db, block, id);
	printf("id = %d; n = %d; ", id, get_n(block));
	printf("leaf = %d; num = %d\n", get_leaf(block), get_num(block));
	for (i = 0; i < get_n(block); ++i) {
		get_data(block, i, &key, &value);
		printf("(key = ");
		for (j = 0; j < key.size; j++)
			printf("%c", ((unsigned char *) key.data)[j]);
		printf(", value = ");
		for (j = 0; j < value.size; j++)
			printf("%c", ((unsigned char *) value.data)[j]);
		printf(", cl = %d ", get_c(block, i));
		printf(", cr = %d) ", get_c(block, i + 1));
	}
	printf("\n\n");
	if (block)
		free(block);
}
