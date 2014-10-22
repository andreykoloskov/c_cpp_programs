#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libdb.h"

int
main(int argc, char *argv[])
{
	const struct DBC conf = { 0, 0 };
	struct DB *db;
	int i;

	if (argc < 2)
		return 0;

	db = dbcreate(argv[1], conf);
	db->close(db);

	db = dbopen(argv[1], conf);

	struct DBT key, data;
	key.data = malloc(KEY_SIZE);
	data.data = malloc(VALUE_SIZE);
	size_t sz;

	strcpy(key.data, "k18"); strcpy(data.data, "v18");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k0"); strcpy(data.data, "v0");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k15"); strcpy(data.data, "v15");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k12"); strcpy(data.data, "v12");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k1"); strcpy(data.data, "v1");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k2"); strcpy(data.data, "v2");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k3"); strcpy(data.data, "v3");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k4"); strcpy(data.data, "v4");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k5"); strcpy(data.data, "v5");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k6"); strcpy(data.data, "v6");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k7"); strcpy(data.data, "v7");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k8"); strcpy(data.data, "v8");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k9"); strcpy(data.data, "v9");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k10"); strcpy(data.data, "v10");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k11"); strcpy(data.data, "v11");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k13"); strcpy(data.data, "v13");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k14"); strcpy(data.data, "v14");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k16"); strcpy(data.data, "v16");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k17"); strcpy(data.data, "v17");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k19"); strcpy(data.data, "v19");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k20"); strcpy(data.data, "v20");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k01"); strcpy(data.data, "v01");
	db_put(db, key.data, 3, data.data, 3);

	strcpy(key.data, "k14"); strcpy(data.data, "v14dubl");
	db_put(db, key.data, 3, data.data, 7);
	strcpy(key.data, "k17"); strcpy(data.data, "v17dubl");
	db_put(db, key.data, 3, data.data, 7);

	strcpy(key.data, "k30"); strcpy(data.data, "v30");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k29"); strcpy(data.data, "v29");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k28"); strcpy(data.data, "v28");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k27"); strcpy(data.data, "v27");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k26"); strcpy(data.data, "v26");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k25"); strcpy(data.data, "v25");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k24"); strcpy(data.data, "v24");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k23"); strcpy(data.data, "v23");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k22"); strcpy(data.data, "v22");
	db_put(db, key.data, 3, data.data, 3);

	strcpy(key.data, "k166"); strcpy(data.data, "v166");
	db_put(db, key.data, 4, data.data, 4);


	strcpy(key.data, "d18"); strcpy(data.data, "v18");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "d0"); strcpy(data.data, "v0");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "d15"); strcpy(data.data, "v15");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "d12"); strcpy(data.data, "v12");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "d1"); strcpy(data.data, "v1");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "d2"); strcpy(data.data, "v2");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "d3"); strcpy(data.data, "v3");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "d4"); strcpy(data.data, "v4");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "d5"); strcpy(data.data, "v5");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "d6"); strcpy(data.data, "v6");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "m7"); strcpy(data.data, "v7");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "m8"); strcpy(data.data, "v8");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "m9"); strcpy(data.data, "v9");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "m10"); strcpy(data.data, "v10");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "m11"); strcpy(data.data, "v11");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "m13"); strcpy(data.data, "v13");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "m14"); strcpy(data.data, "v14");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "m16"); strcpy(data.data, "v16");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "m17"); strcpy(data.data, "v17");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "m19"); strcpy(data.data, "v19");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "m20"); strcpy(data.data, "v20");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "m01"); strcpy(data.data, "v01");
	db_put(db, key.data, 3, data.data, 3);

	if (data.data) free(data.data);
	strcpy(key.data, "k17");
	if (db_get(db, key.data, 3, &(data.data), &sz) != -1)
		printf("%c%c %d %c%c %d\n", ((char *)(key.data))[0], ((char *)(key.data))[1], 3, ((char *)(data.data))[0], ((char *)(data.data))[1], sz);
	if (data.data) free(data.data);
	strcpy(key.data, "k11");
	if (db_get(db, key.data, 3, &(data.data), &sz) != -1)
		printf("%c%c %d %c%c %d\n", ((char *)(key.data))[0], ((char *)(key.data))[1], 3, ((char *)(data.data))[0], ((char *)(data.data))[1], sz);
	if (data.data) free(data.data);
	strcpy(key.data, "k4");
	if (db_get(db, key.data, 2, &(data.data), &sz) != -1)
		printf("%c%c %d %c%c %d\n", ((char *)(key.data))[0], ((char *)(key.data))[1], 2, ((char *)(data.data))[0], ((char *)(data.data))[1], sz);
	if (data.data) free(data.data);
	strcpy(key.data, "k128");
	if (db_get(db, key.data, 4, &(data.data), &sz) != -1)
		printf("%c%c %d %c%c %d\n", ((char *)(key.data))[0], ((char *)(key.data))[1], 4, ((char *)(data.data))[0], ((char *)(data.data))[1], sz);

	print_db(db);

	strcpy(key.data, "k18");
	db_del(db, key.data, 3);
	strcpy(key.data, "k19");
	db_del(db, key.data, 3);
	strcpy(key.data, "k2");
	db_del(db, key.data, 2);

	strcpy(key.data, "k10");
	db_del(db, key.data, 3);
	strcpy(key.data, "k29");
	db_del(db, key.data, 3);
	strcpy(key.data, "m9");
	db_del(db, key.data, 2);

	strcpy(key.data, "d0");
	db_del(db, key.data, 2);
	strcpy(key.data, "d1");
	db_del(db, key.data, 2);
	strcpy(key.data, "d12");
	db_del(db, key.data, 3);

	strcpy(key.data, "d15");
	db_del(db, key.data, 3);
	strcpy(key.data, "k1");
	db_del(db, key.data, 2);
	strcpy(key.data, "k28");
	db_del(db, key.data, 3);
	strcpy(key.data, "m10");
	db_del(db, key.data, 3);
	strcpy(key.data, "k8");
	db_del(db, key.data, 2);
	strcpy(key.data, "k9");
	db_del(db, key.data, 2);
	strcpy(key.data, "k15");
	db_del(db, key.data, 3);
	strcpy(key.data, "k14");
	db_del(db, key.data, 3);
	strcpy(key.data, "k13");
	db_del(db, key.data, 3);
	strcpy(key.data, "k12");
	db_del(db, key.data, 3);
	strcpy(key.data, "k7");
	db_del(db, key.data, 2);
	strcpy(key.data, "k16");
	db_del(db, key.data, 3);
	strcpy(key.data, "k166");
	db_del(db, key.data, 4);
	strcpy(key.data, "m20");
	db_del(db, key.data, 3);
	strcpy(key.data, "m11");
	db_del(db, key.data, 3);
	strcpy(key.data, "m19");
	db_del(db, key.data, 3);
	strcpy(key.data, "d18");
	db_del(db, key.data, 3);
	strcpy(key.data, "k30");
	db_del(db, key.data, 3);
	strcpy(key.data, "k4");
	db_del(db, key.data, 2);
	strcpy(key.data, "k6");
	db_del(db, key.data, 2);
	strcpy(key.data, "k17");
	db_del(db, key.data, 3);
	strcpy(key.data, "k22");
	db_del(db, key.data, 3);
	strcpy(key.data, "k23");
	db_del(db, key.data, 3);
	strcpy(key.data, "k24");
	db_del(db, key.data, 3);
	strcpy(key.data, "k20");
	db_del(db, key.data, 3);
	strcpy(key.data, "m14");
	db_del(db, key.data, 3);
	strcpy(key.data, "m16");
	db_del(db, key.data, 3);
	strcpy(key.data, "m17");
	db_del(db, key.data, 3);
	strcpy(key.data, "m7");
	db_del(db, key.data, 2);
	strcpy(key.data, "d6");
	db_del(db, key.data, 2);
	strcpy(key.data, "d5");
	db_del(db, key.data, 2);
	strcpy(key.data, "d4");
	db_del(db, key.data, 2);
	strcpy(key.data, "d3");
	db_del(db, key.data, 2);
	strcpy(key.data, "d2");
	db_del(db, key.data, 2);
	strcpy(key.data, "u7");
	db_del(db, key.data, 2);
	strcpy(key.data, "k0");
	db_del(db, key.data, 2);
	strcpy(key.data, "k01");
	db_del(db, key.data, 3);
	strcpy(key.data, "k11");
	db_del(db, key.data, 3);
	strcpy(key.data, "k3");
	db_del(db, key.data, 2);
	strcpy(key.data, "k25");
	db_del(db, key.data, 3);
	strcpy(key.data, "k26");
	db_del(db, key.data, 3);
	strcpy(key.data, "k27");
	db_del(db, key.data, 3);
	strcpy(key.data, "k5");
	db_del(db, key.data, 2);
	strcpy(key.data, "m13");
	db_del(db, key.data, 3);
	strcpy(key.data, "m8");
	db_del(db, key.data, 2);
	strcpy(key.data, "m01");
	db_del(db, key.data, 3);

	data.data = malloc(VALUE_SIZE);

	strcpy(key.data, "m11"); strcpy(data.data, "v11");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "a1"); strcpy(data.data, "v1");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k18"); strcpy(data.data, "v18");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k0"); strcpy(data.data, "v0");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k15"); strcpy(data.data, "v15");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k12"); strcpy(data.data, "v12");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k1"); strcpy(data.data, "v1");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k2"); strcpy(data.data, "v2");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k3"); strcpy(data.data, "v3");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k4"); strcpy(data.data, "v4");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k5"); strcpy(data.data, "v5");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k6"); strcpy(data.data, "v6");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k7"); strcpy(data.data, "v7");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k8"); strcpy(data.data, "v8");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k9"); strcpy(data.data, "v9");
	db_put(db, key.data, 2, data.data, 2);
	strcpy(key.data, "k10"); strcpy(data.data, "v10");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k11"); strcpy(data.data, "v11");
	db_put(db, key.data, 3, data.data, 3);
	strcpy(key.data, "k13"); strcpy(data.data, "v13");
	db_put(db, key.data, 3, data.data, 3);

	if (data.data) free(data.data);
		strcpy(key.data, "k4");
		if (db_get(db, key.data, 2, &(data.data), &sz) != -1)
			printf("%c%c %d %c%c %d\n", ((char *)(key.data))[0], ((char *)(key.data))[1], 2, ((char *)(data.data))[0], ((char *)(data.data))[1], sz);

	strcpy(key.data, "m20");
	db_del(db, key.data, 3);
	strcpy(key.data, "k19");
	db_del(db, key.data, 3);
	strcpy(key.data, "d0");
	db_del(db, key.data, 2);
	strcpy(key.data, "k29");
	db_del(db, key.data, 3);
	strcpy(key.data, "k18");
	db_del(db, key.data, 3);
	strcpy(key.data, "k3");
	db_del(db, key.data, 2);
	strcpy(key.data, "k4");
	db_del(db, key.data, 2);
	strcpy(key.data, "k5");
	db_del(db, key.data, 2);
	strcpy(key.data, "a1");
	db_del(db, key.data, 2);
	strcpy(key.data, "k0");
	db_del(db, key.data, 2);
	strcpy(key.data, "k1");
	db_del(db, key.data, 2);
	strcpy(key.data, "k10");
	db_del(db, key.data, 3);
	strcpy(key.data, "k11");
	db_del(db, key.data, 3);
	strcpy(key.data, "k12");
	db_del(db, key.data, 3);
	strcpy(key.data, "k13");
	db_del(db, key.data, 3);
	strcpy(key.data, "k15");
	db_del(db, key.data, 3);
	strcpy(key.data, "k2");
	db_del(db, key.data, 2);
	strcpy(key.data, "k6");
	db_del(db, key.data, 2);
	strcpy(key.data, "k8");
	db_del(db, key.data, 2);
	strcpy(key.data, "k9");
	db_del(db, key.data, 2);
	strcpy(key.data, "k7");
	db_del(db, key.data, 2);
	strcpy(key.data, "m11");
	db_del(db, key.data, 3);

	if (data.data) free(data.data);
	strcpy(key.data, "k2");
	if (db_get(db, key.data, 2, &(data.data), &sz) != -1)
		printf("%s %d %s %d\n", key.data, key.size, data.data, data.size);

	print_db(db);


	if (key.data) free(key.data);
	if (data.data) free(data.data);

	db->close(db);

	return 0;
}
