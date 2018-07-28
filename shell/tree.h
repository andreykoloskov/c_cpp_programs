#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <vector>
#include <stack>
#include "lexem.h"
#include "parser.h"

using std::vector;
using std::stack;

struct Node
{
	Lexem lexem;
	Node *left;
	Node *right;
};

class Tree
{
private:
	Node *root;
	stack<Node> v_stack;
public:
	Tree() { root = nullptr; }
	~Tree();
	void add_node(Lexem lexem);
	Node * get_root() { return root; }
	void set_root();
	void print_tree(Node *root);
};

#endif
