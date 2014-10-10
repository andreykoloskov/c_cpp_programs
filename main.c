#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libdb.h"

int
main(int argc, char **argv)
{
	const struct DBC conf = { 0, 0, 0 };
	struct DB *db;
	int i;

	if (argc < 2)
		return 0;

	db = dbcreate(argv[1], &conf);
	db->close(db);

	db = dbopen(argv[1], &conf);

	printf("%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", db->head.db_size, db->head.chunk_size, db->head.mem_size, \
			db->head.stat_offset, db->head.stat_count, db->head.data_offset, db->head.data_count, db->head.root_id, db->head.t, \
			((struct NodeHead *)db->root)->n, ((struct NodeHead *)db->root)->leaf, ((struct NodeHead *)db->root)->num);

	struct DBT key, data;
	key.data = (unsigned char *) malloc(KEY_SIZE);
	data.data = (unsigned char *) malloc(VALUE_SIZE);

	strcpy(key.data, "k1"); strcpy(data.data, "v1");
	db->put(db, &key, &data);
	strcpy(key.data, "k2"); strcpy(data.data, "v2");
	db->put(db, &key, &data);
	strcpy(key.data, "k3"); strcpy(data.data, "v3");
	db->put(db, &key, &data);
	strcpy(key.data, "k4"); strcpy(data.data, "v4");
	db->put(db, &key, &data);
	strcpy(key.data, "k5"); strcpy(data.data, "v5");
	db->put(db, &key, &data);
	strcpy(key.data, "k6"); strcpy(data.data, "v6");
	db->put(db, &key, &data);
	strcpy(key.data, "k15"); strcpy(data.data, "v15");
	db->put(db, &key, &data);
	strcpy(key.data, "k7"); strcpy(data.data, "v7");
	db->put(db, &key, &data);
	strcpy(key.data, "k8"); strcpy(data.data, "v8");
	db->put(db, &key, &data);
	strcpy(key.data, "k9"); strcpy(data.data, "v9");
	db->put(db, &key, &data);
	strcpy(key.data, "k10"); strcpy(data.data, "v10");
	db->put(db, &key, &data);
	strcpy(key.data, "k11"); strcpy(data.data, "v11");
	db->put(db, &key, &data);
	strcpy(key.data, "k12"); strcpy(data.data, "v12");
	db->put(db, &key, &data);
	strcpy(key.data, "k13"); strcpy(data.data, "v13");
	db->put(db, &key, &data);
	strcpy(key.data, "k14"); strcpy(data.data, "v14");
	db->put(db, &key, &data);
	strcpy(key.data, "k16"); strcpy(data.data, "v16");
	db->put(db, &key, &data);
	strcpy(key.data, "k17"); strcpy(data.data, "v17");
	db->put(db, &key, &data);
	strcpy(key.data, "k18"); strcpy(data.data, "v18");
	db->put(db, &key, &data);
	strcpy(key.data, "k19"); strcpy(data.data, "v19");
	db->put(db, &key, &data);

	//strcpy(key.data, "k20"); strcpy(data.data, "v20");
	//db->put(db, &key, &data);


	if (key.data) free(key.data);
	if (data.data) free(data.data);

	print_db(db);

	db->close(db);

	return 0;
}
