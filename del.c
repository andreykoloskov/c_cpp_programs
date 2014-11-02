#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libdb.h"

//Удаление
int db_del(struct DB *db, void *key, size_t key_len)
{
	struct DBT keyt = {
		.data = key,
		.size = key_len
	};
	return db->del(db, &keyt);
}

//Внутренняя реализация удаления
int del(struct DB *db, struct DBT *key)
{
	return internal_del(db, db->root, key);
}

int internal_del(struct DB *db, Block block, struct DBT *key)
{
	struct DBT key2, value2;
	Block x = (Block) malloc(db->head.chunk_size);
	memcpy(x, block, db->head.chunk_size);
	int i, t = db->head.t;
	if (get_leaf(x))
		del_1(db, x, key);  /*1*/
	else {
		for (i = 0; i < get_n(x); i++) {
			get_data(x, i, &key2, &value2);
			if (!cmp(key, &key2)) {
				int sz = db->head.chunk_size;
				Block y = (Block) malloc(sz);
				Block z = (Block) malloc(sz);
				Block dop = (Block) malloc(sz);
				db_block_read(db, y, get_c(x, i));
				db_block_read(db, z, get_c(x, i + 1));
				if (get_n(y) >= t)
					del_2a(db, x, y, dop, i);  /*2a*/
				else if (get_n(z) >= t)
					del_2b(db, x, z, dop, i);  /*2b*/
				else
					del_2c(db, x, y, z, dop, i);  /*2c*/

				if (y) {
					free(y);
					y = NULL;
				}
				if (z) {
					free(z);
					z = NULL;
				}
				if (dop) {
					free(dop);
					dop = NULL;
				}
				if (x) {
					free(x);
					x = NULL;
				}

				return 0;
			}
		}

		i = 0;
		while (i < get_n(x)) {
			get_data(x, i, &key2, &value2);
			if (cmp(key, &key2) <= 0)
				break;
			i++;
		}

		Block c = (Block) malloc(db->head.chunk_size);
		Block l = (Block) malloc(db->head.chunk_size);
		Block r = (Block) malloc(db->head.chunk_size);

		db_block_read(db, c, get_c(x, i));

		if (i > 0)
			db_block_read(db, l, get_c(x, i - 1));
		if (i < get_n(x))
			db_block_read(db, r, get_c(x, i + 1));
		if (get_n(c) <= t - 1) {
			if (i > 0 && get_n(l) >= t)
				del_3a_l(db, x, c, l, i);   /*3a_l*/
			else if (i < get_n(x) && get_n(r) >= t)
				del_3a_r(db, x, c, r, i);   /*3a_r*/
			else
				del_3b(db, x, c, l, r, i);   /*3b*/
		}
		//Рекурсивно удаляем k из поддерева
		//с корнем ci[x]
		internal_del(db, c, key);

		if (c) {
			free(c);
			c = NULL;
		}
		if (l) {
			free(l);
			l = NULL;
		}
		if (r) {
			free(r);
			r = NULL;
		}
		if (x) {
			free(x);
			x = NULL;
		}
	}

	return 0;
}

//1. Если: x - лист, k принадлежит x => удаляем k
// В x всегда будет не менее T элементов
// (благодаря п. 3)
void del_1(struct DB *db, Block x, struct DBT *key)
{
	int i, j;
	for (i = 0; i < get_n(x); i++) {
		struct DBT key1, value1;
		get_data(x,i, &key1, &value1);
		if (!cmp(key, &key1)) {
			for (j = i + 1; j < get_n(x); j++) {
				get_data(x, j, &key1, &value1);
				set_data(x, j - 1, &key1, &value1);
			}
			set_n(x, get_n(x) - 1);
			db_block_write(db, x, get_num(x));
		}
	}
	//Если элементов в базе нет
	if (!get_n(x))
		db_block_free_root(db, get_num(x));
}

//2. Если: x - внутренний узел, k принадлежит x =>
//  a) Если: y - дочерний к x, непосредственно
//     предшествует k,
//	y - содержит не менее T ключей =>
//		- Находим k' - предшественник k
//			в поддереве с корнем y;
//		- Рекурсивно удаляем k';
//		- Заменяем k в x ключем k';
void del_2a(struct DB *db, Block x, Block y, Block dop, int i)
{
	memcpy(dop, y, db->head.chunk_size);
	while (!get_leaf(dop))
		db_block_read(db, dop, get_c(dop, get_n(dop)));
	struct DBT key1, value1;
	get_data(dop, get_n(dop) - 1, &key1, &value1);
	internal_del(db, y, &key1);
	set_data(x, i, &key1, &value1);
	db_block_write(db, x, get_num(x));
}

//2. Если: x - внутренний узел, k принадлежит x =>
//  b) Если: z - дочерний к x, непосредственно
//     следует за k,
//	z - содержит не менее T ключей =>
//		- Находим k' - следующий за k
//			в поддереве с корнем z;
//		- Рекурсивно удаляем k';
//		- Заменяем k в x ключем k';
void del_2b(struct DB *db, Block x, Block z, Block dop, int i)
{
	memcpy(dop, z, db->head.chunk_size);
	while (!get_leaf(dop))
		db_block_read(db, dop, get_c(dop, 0));
	struct DBT key1, value1;
	get_data(dop, 0, &key1, &value1);
	set_data(x, i, &key1, &value1);
	db_block_write(db, x, get_num(x));
	internal_del(db, z, &key1);
}

//2. Если: x - внутренний узел,
//   k принадлежит x =>
//  c) Если: y и z - содержат по T - 1 ключей =>
//      - В y вносим k и все ключи из z;
//      - Из x удаляем k и указатель на z;
//      - Если x - корень, и в нем не остается
//        ключей =>
//          -- y - новый корень;
//      - Освобождаем z;
//      - Рекурсиво удаляем k из y;
void del_2c(struct DB *db, Block x, Block y, Block z, Block dop, int i)
{
	int j, k;
	struct DBT key, value, key1, value1;
	get_data(x, i, &key, &value);
	set_n(y, get_n(y) + 1);
	k = get_n(y) - 1;
	set_data(y, k, &key, &value);
	for (j = get_n(y); j < get_n(y) + get_n(z); j++) {
		get_data(z, j - get_n(y), &key1, &value1);
		set_data(y, j, &key1, &value1);
	}
	for (j = get_n(y); j <= get_n(y) + get_n(z); j++)
		set_c(y, j, get_c(z, j - get_n(y)));
	set_n(y, get_n(y) + get_n(z));
	set_n(z, 0);
	for (j = i; j < get_n(x) - 1; j++) {
		get_data(x, j + 1, &key1, &value1);
		set_data(x, j, &key1, &value1);
	}
	for (j = i + 1; j < get_n(x); j++)
		set_c(x, j, get_c(x, j + 1));
	set_n(x, get_n(x) - 1);

	if (!get_n(x)) {
		//Получился нулевой корень
		//Это может быть только корень,
		//т.к. в ином случае 0 не получится
		db_block_free(db, db->head.root_id);
		db->head.root_id = get_num(y);
		db_block_write(db, y, get_num(y));
	} else {
		db_block_write(db, x, get_num(x));
		db_block_write(db, y, get_num(y));
	}
	db_block_write(db, z, get_num(z));
	db_block_free(db, get_num(z));
	get_data(y, k, &key, &value);
	internal_del(db, y, &key);
}

//3. Если: x - внутренний узел,
//	k - отсутствует в x =>
//      - Находим ci[x] - корень поддерева,
//        которое должно содержать k
//      a) Если: ci[x] - содержит T - 1 ключей,
//        левый сосед из x содержит
//        не менее T ключей =>
//          -- В ci[x] передадим ключ
//             между ci[x] и этим соседом из x;
//          -- На место этого ключа переместим
//		крайний ключ из соседнего узла;
//          -- Перенесем указатель из соседнего
//		узла в ci[x];
void del_3a_l(struct DB *db, Block x, Block c, Block l, int i)
{
	int j;
	struct DBT key, value;
	set_n(c, get_n(c) + 1);
	for (j = get_n(c) - 1; j > 0; j--) {
		get_data(c, j - 1, &key, &value);
		set_data(c, j, &key, &value);
	}
	for (j = get_n(c); j > 0; j--)
		set_c(c, j, get_c(c, j - 1));

	get_data(x, i - 1, &key, &value);
	set_data(c, 0, &key, &value);
	set_c(c, 0, get_c(l, get_n(l)));

	get_data(l, get_n(l) - 1, &key, &value);
	set_data(x, i - 1, &key, &value);
	set_n(l, get_n(l) - 1);

	db_block_write(db, l, get_num(l));
	db_block_write(db, x, get_num(x));
	db_block_write(db, c, get_num(c));
}

//3. Если: x - внутренний узел,
//   k - отсутствует в x =>
//      - Находим ci[x] - корень поддерева,
//        которое должно содержать k (если есть)
//      a) Если: ci[x] - содержит T - 1 ключей,
//         правый сосед из x содержит не менее
//         T ключей =>
//          -- В ci[x] передадим ключ
//             между ci[x] и этим соседом из x;
//          -- На место этого ключа переместим
//             крайний ключ из соседнего узла;
//          -- Перенесем указатель из соседнего
//             узла в ci[x];
void del_3a_r(struct DB *db, Block x, Block c, Block r, int i)
{
	int j;
	struct DBT key, value;
	set_n(c, get_n(c) + 1);
	get_data(x, i, &key, &value);
	set_data(c, get_n(c) - 1, &key, &value);
	set_c(c, get_n(c), get_c(r, 0));

	get_data(r, 0, &key, &value);
	set_data(x, i, &key, &value);
	for (j = 1; j < get_n(r); j++) {
		get_data(r, j, &key, &value);
		set_data(r, j - 1, &key, &value);
	}
	for (j = 1; j <= get_n(r); j++)
		set_c(r, j - 1, get_c(r, j));
	set_n(r, get_n(r) - 1);
	db_block_write(db, r, get_num(r));
	db_block_write(db, x, get_num(x));
	db_block_write(db, c, get_num(c));
}

//3. Если: x - внутренний узел,
//   k - отсутствует в x =>
//      b) Если: ci[x] и оба соседа из x
//         содержат по T - 1 ключей =>
//          -- В ci[x] добавим ключ - разделитель
//             из x и одного (напр. левого) соседа;
//          -- Если x - корень,
//             и в нем не остается ключей =>
//              --- ci[x] - новый корень;
void del_3b(struct DB *db, Block x, Block c, Block l, Block r, int i)
{
	int j;
	int t = db->head.t;
	struct DBT key, value;
	if (i > 0 && get_n(l) < t) {
		set_n(c, get_n(c) + get_n(l) + 1);
		for (j = get_n(l) + 1; j < get_n(c); j++) {
			get_data(c, j - get_n(l) - 1, &key, &value);
			set_data(c, j, &key, &value);
		}
		for (j = get_n(l) + 1; j <= get_n(c); j++)
			set_c(c, j, get_c(c, j - get_n(l) - 1));

		for (j = 0; j < get_n(l); j++) {
			get_data(l, j, &key, &value);
			set_data(c, j, &key, &value);
		}
		for (j = 0; j <= get_n(l); j++)
			set_c(c, j, get_c(l, j));

		get_data(x, i - 1, &key, &value);
		set_data(c, get_n(l), &key, &value);
		set_n(l, 0);

		for (j = i - 1; j < get_n(x) - 1; j++) {
			get_data(x, j + 1, &key, &value);
			set_data(x, j, &key, &value);
		}
		for (j = i - 1; j < get_n(x); j++)
			set_c(x, j, get_c(x, j + 1));
		set_n(x, get_n(x) - 1);

		db_block_write(db, l, get_num(l));
		db_block_free(db, get_num(l));
	} else if (i < get_n(x) && get_n(r) < t) {
		get_data(x, i, &key, &value);
		set_data(c, get_n(c), &key, &value);
		set_n(c, get_n(c) + 1);

		for (j = get_n(c); j < get_n(c) + get_n(r); j++) {
			get_data(r, j - get_n(c), &key, &value);
			set_data(c, j, &key, &value);
		}
		for (j = get_n(c); j <= get_n(c) + get_n(r); j++)
			set_c(c, j, get_c(r, j - get_n(c)));
		set_n(c, get_n(c) + get_n(r));

		set_n(r, 0);
		for (j = i; j < get_n(x) - 1; j++) {
			get_data(x, j + 1, &key, &value);
			set_data(x, j, &key, &value);
		}
		for (j = i + 1; j < get_n(x); j++)
			set_c(x, j, get_c(x, j + 1));

		set_n(x, get_n(x) - 1);
		db_block_write(db, r, get_num(r));
		db_block_free(db, get_num(r));
	}
	if (!get_n(x)) {
		//Получился нулевой корень
		//Это может быть только корень,
		//т.к. в ином случае 0 не получится
		db_block_free(db, db->head.root_id);
		db->head.root_id = get_num(c);
		db_block_write(db, c, get_num(c));
	} else {
		db_block_write(db, x, get_num(x));
		db_block_write(db, c, get_num(c));
	}
}
