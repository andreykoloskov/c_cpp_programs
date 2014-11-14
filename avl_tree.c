#include <stdio.h>
#include <stdlib.h>
#include "libdb.h"

int height(Node *t) //Высота поддерева с коренем t
{
	return t ? t -> height : 0;
}

int bfactor(Node *t) //Разность высот поддеревьев узла t
{
	return height(t -> right) - height(t -> left);
}

void fixheight(Node *t) //Запись высоты в узел
{
	int hl = height(t -> left);
	int hr = height(t -> right);
	t -> height = (hl > hr ? hl : hr) + 1;
}

Node *rotateright(Node *t) //Правый поворот вокруг t
{
	Node *q = t -> left;
	t -> left = q -> right;
	q -> right = t;
	fixheight(t);
	fixheight(q);
	return q;
}

Node *rotateleft(Node *t) //Левый поворот вокруг t
{
	Node *p = t -> right;
	t -> right = p -> left;
	p -> left = t;
	fixheight(t);
	fixheight(p);
	return p;
}

Node *balance(Node *t) //Балансировка узла t
{
	fixheight(t);
	if(bfactor(t) == 2) {
		if(bfactor(t -> right) < 0)
			t -> right = rotateright(t->right);
		return rotateleft(t);
	}
	if(bfactor(t) == -2) {
		if(bfactor(t -> left) > 0)
			t -> left = rotateleft(t -> left);
		return rotateright(t);
	}
	return t; //Балансировка не нужна
}

Node *insert(Node *t, int key, int data) //Вставка ключа key в дерево с корнем t
{
	if (t == NULL) {
		t = (Node *) malloc(sizeof(*t));
		t -> key = key;
		t -> data = data;
		t -> left = NULL;
		t -> right = NULL;
		t -> height = 1;
	} else if( key < t -> key )
		t -> left = insert(t -> left, key, data);
	else
		t -> right = insert(t -> right, key, data);
	return balance(t);
}

Node *findmin(Node *t) //Поиск узла с минимальным ключом в дереве t
{
	if (t == NULL)
		return NULL;
	else if (t -> left == NULL)
		return t;
	else
		return findmin(t -> left);
}

Node *findmax(Node *t) //Поиск узла с максимальным ключом в дереве t
{
	if (t != NULL)
		while (t -> right != NULL)
			t = t -> right;
	return t;
}

Node *removemin(Node *t) //Удаление узла с минимальным ключом из дерева t
{
	if(t -> left == 0)
		return t -> right;
	t -> left = removemin(t -> left);
	return balance(t);
}

Node *find(Node *t, int key) //Поиск заданного ключа
{
	if (t == NULL || key == t -> key)
		return t;
	if (key < t -> key)
		return find(t -> left, key);
	else
		return find(t -> right, key);
}

Node *remove_tree(Node *t, int key) //Удаление ключа key из дерева t
{
	if (t == NULL )
		return NULL;
	if (key < t -> key )
		t -> left = remove_tree(t -> left, key);
	else if (key > t -> key )
		t -> right = remove_tree(t -> right, key);
	else {
		Node *q = t -> left;
		Node *r = t -> right;
		free(t);
		t = NULL;
		if (r == NULL )
			return q;
		Node *min = findmin(r);
		min -> right = removemin(r);
		min -> left = q;
		return balance(min);
	}
	return balance(t);
}


//Освобождение памяти
Node* tfree(Node *root)
{
	if (root) {
		tfree(root -> left);
		tfree(root -> right);
		free(root);
	}
	return NULL;
}

//Просмотр в прямом порядке
void print_tree(Node *root)
{
	if (root) {
		print_tree(root->left);
		printf("(%d, %d)", root->key, root->data);
		print_tree(root->right);
	}
}
