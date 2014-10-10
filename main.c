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

	struct DBT key, data;
	key.data = (unsigned char *) malloc(KEY_SIZE);
	data.data = (unsigned char *) malloc(VALUE_SIZE);

	strcpy(key.data, "k18"); strcpy(data.data, "v18");
	db->put(db, &key, &data);
	strcpy(key.data, "k0"); strcpy(data.data, "v0");
	db->put(db, &key, &data);
	strcpy(key.data, "k15"); strcpy(data.data, "v15");
	db->put(db, &key, &data);
	strcpy(key.data, "k12"); strcpy(data.data, "v12");
	db->put(db, &key, &data);
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
	strcpy(key.data, "k13"); strcpy(data.data, "v13");
	db->put(db, &key, &data);
	strcpy(key.data, "k14"); strcpy(data.data, "v14");
	db->put(db, &key, &data);
	strcpy(key.data, "k16"); strcpy(data.data, "v16");
	db->put(db, &key, &data);
	strcpy(key.data, "k17"); strcpy(data.data, "v17");
	db->put(db, &key, &data);
	strcpy(key.data, "k19"); strcpy(data.data, "v19");
	db->put(db, &key, &data);
	strcpy(key.data, "k20"); strcpy(data.data, "v20");
	db->put(db, &key, &data);
	strcpy(key.data, "k01"); strcpy(data.data, "v01");
	db->put(db, &key, &data);

	strcpy(key.data, "k14"); strcpy(data.data, "v14dubl");
	db->put(db, &key, &data);
	strcpy(key.data, "k17"); strcpy(data.data, "v17dubl");
	db->put(db, &key, &data);

	strcpy(key.data, "k30"); strcpy(data.data, "v30");
	db->put(db, &key, &data);
	strcpy(key.data, "k29"); strcpy(data.data, "v29");
	db->put(db, &key, &data);
	strcpy(key.data, "k28"); strcpy(data.data, "v28");
	db->put(db, &key, &data);
	strcpy(key.data, "k27"); strcpy(data.data, "v27");
	db->put(db, &key, &data);
	strcpy(key.data, "k26"); strcpy(data.data, "v26");
	db->put(db, &key, &data);
	strcpy(key.data, "k25"); strcpy(data.data, "v25");
	db->put(db, &key, &data);
	strcpy(key.data, "k24"); strcpy(data.data, "v24");
	db->put(db, &key, &data);
	strcpy(key.data, "k23"); strcpy(data.data, "v23");
	db->put(db, &key, &data);
	strcpy(key.data, "k22"); strcpy(data.data, "v22");
	db->put(db, &key, &data);

	strcpy(key.data, "k166"); strcpy(data.data, "v166");
	db->put(db, &key, &data);

	strcpy(key.data, "k17");
	if (db->get(db, &key, &data) != -1)
		printf("%s %s\n", key.data, data.data);
	strcpy(key.data, "k11");
	if (db->get(db, &key, &data) != -1)
		printf("%s %s\n", key.data, data.data);
	strcpy(key.data, "k4");
	if (db->get(db, &key, &data) != -1)
		printf("%s %s\n", key.data, data.data);
	strcpy(key.data, "k128");
	if (db->get(db, &key, &data) != -1)
		printf("%s %s\n", key.data, data.data);


	if (key.data) free(key.data);
	if (data.data) free(data.data);

	print_db(db);

	db->close(db);

	return 0;
}
