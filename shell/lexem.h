#ifndef LEXEM_H
#define LEXEM_H

#include <iostream>
#include <string>
#include <cstring>

class Lexem
{
public:
	static const int str_len = 1024;
	static const int attr_cnt = 256;
	enum TYPE {TYPE_NULL, COMMAND, OPERATION};
	enum OPNUM
	{
		OP_NULL,
		OP1 /*"("*/, OP2/*")"*/,
		OP3/*"<"*/, OP4/*">"*/, OP5/*">>"*/,
		OP6/*"|"*/,
		OP7/*"||"*/, OP8 /*"&"*/, OP9/*"&&"*/
	};
private:
	TYPE type;
	OPNUM op_num;
	int level;
	char str[str_len];
	char *attr[attr_cnt];

	int set_lexem_command(TYPE tp, OPNUM op, int lev, char ch, int ret);
public:
	Lexem();
	Lexem(const Lexem &lexem);
	~Lexem() {}
	TYPE get_type() { return type; }
	OPNUM get_op_num() { return op_num; }
	int get_level() { return level; }
	char* get_str() { return str; }
	char** get_attr() { return attr; }
	void set_attr();
	int get_lexem(const char ch);
	friend std::ostream & operator<<(std::ostream & os, Lexem &lexem);
	Lexem & operator=(const Lexem &lexem);
	bool operator>(Lexem &lexem);
	bool operator<(Lexem &lexem);
	bool operator==(Lexem &lexem);
	void clear_lexem();
};

#endif
