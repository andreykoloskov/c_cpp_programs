#include "lexem.h"
#include "tree.h"

void Tree::add_node(Lexem lexem)
{
	Node node;
	node.lexem = lexem;
	node.left = nullptr;
	node.right = nullptr;

	if (lexem.get_type() == Lexem::COMMAND)
		v_stack.push(node);
	else if (lexem.get_type() == Lexem::OPERATION)
	{
		if (lexem.get_op_num() != Lexem::OP8)
		{
			if (v_stack.size() < 2)
				exit(1);

			Node *nd1 = new Node(v_stack.top());
			v_stack.pop();
			Node *nd2 = new Node(v_stack.top());
			v_stack.pop();

			node.left = nd2;
			node.right = nd1;
			v_stack.push(node);
		}
		else
		{
			if (!v_stack.size())
				exit(1);

			Node *nd1 = new Node(v_stack.top());
			v_stack.pop();
			node.left = nd1;
			node.right = nullptr;
			v_stack.push(node);
		}
	}
}

void Tree::set_root()
{
	root = new Node(v_stack.top());
	v_stack.pop();
}

static void dell_tree(Node *root)
{
	if (root->left)
		dell_tree(root->left);
	if (root->right)
		dell_tree(root->right);
	delete root;
}

Tree::~Tree()
{
	dell_tree(root);
}

void Tree::print_tree(Node *root)
{
	if (root->left)
		print_tree(root->left);
	std::cout << root->lexem << std::endl;
	if (root->right)
		print_tree(root->right);
}

