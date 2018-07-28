#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <stack>
#include "lexem.h"

using std::vector;
using std::stack;

class Parser
{
private:
	vector<Lexem> v_lex;
	stack<Lexem> v_stack;
public:
	Parser() {}
	~Parser() {}
	void clear_stack();
	void add_lexem(Lexem lexem);
	vector<Lexem> get_v_lex() { return v_lex; }
	Lexem & operator[](int n) { return v_lex[n]; }
	int size() { return v_lex.size(); }
};

#endif
