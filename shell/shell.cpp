#include <iostream>
#include <string>
#include <cstring>
#include "lexem.h"
#include "parser.h"
#include "tree.h"
#include "worker.h"

using std::string;
using std::cin;
using std::cout;
using std::endl;

int
main(int argc, char **argv)
{
	string str0;
	getline(cin, str0);
	const char *str1 = str0.c_str();

	if (strlen(str1) >= Lexem::str_len)
		return 1;

	Lexem lexem;
	Parser parser;

	for (; str1 && *str1; str1++)
	{
		int res = lexem.get_lexem(*str1);
		if (res == -2)
		{
			cout << "ERROR" << endl;
			return 0;
		}
		else if (res == -1)
		{
			parser.add_lexem(lexem);
			lexem.clear_lexem();
			str1--;
		}
		else if (res == 0)
		{
			continue;
		}
		else if (res == 1)
		{
			parser.add_lexem(lexem);
			lexem.clear_lexem();
		}
	}

	if (strlen(lexem.get_str()))
	{
		lexem.set_attr();
		parser.add_lexem(lexem);
		lexem.clear_lexem();
	}

	parser.clear_stack();

	Tree tree;

	for (int i = 0; i < parser.size(); i++)
		tree.add_node(parser[i]);
	tree.set_root();

	Worker worker(tree.get_root());

	return 0;
}
