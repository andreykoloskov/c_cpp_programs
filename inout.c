#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libdb.h"

void db_block_read(struct DB *db, Block block, int id)
{
	int offset = db->head.data_offset + id;
	fseek(db->fd, offset * db->head.chunk_size, SEEK_SET);
	fread(block, db->head.chunk_size, 1, db->fd);
}

void db_block_write(struct DB *db, Block block, int id)
{
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

int db_block_alloc(struct DB *db)
{
	int i, j;
	int max = db->head.data_count / 8;
	for (i = 0; (i < max) && (db->block_stat[i] == 0xff); i++)
		;

	if (i == max)
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
	int max2 = db->head.stat_count * db->head.chunk_size;
	fwrite(db->block_stat, max2, 1, db->fd);

	return id;
}

void db_block_free(struct DB *db, int id)
{
	db->block_stat[id / 8] &= ~((0x1 << 7) >> (id % 8));
	fseek(db->fd, db->head.stat_offset * db->head.chunk_size, SEEK_SET);
	int max = db->head.stat_count * db->head.chunk_size;
	fwrite(db->block_stat, max, 1, db->fd);
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
