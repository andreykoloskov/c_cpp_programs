#include "lexem.h"

using std::string;
using std::cin;
using std::cout;
using std::endl;

Lexem::Lexem()
{
	type = TYPE_NULL;
	op_num = OP_NULL;
	level = 0;

	for (int i = 0; i < str_len; i++)
		str[i] = '\0';

	attr[0] = str;
	for (int i = 1; i < attr_cnt; i++)
		attr[i] = NULL;
}

Lexem::Lexem(const Lexem &lexem)
{
	type = lexem.type;
	op_num = lexem.op_num;
	level = lexem.level;

	for (int i = 0; i < str_len; i++)
		str[i] = lexem.str[i];

	for (int i = 0; i < attr_cnt; i++)
		if (lexem.attr[i])
			attr[i] = &str[lexem.attr[i] - lexem.str];
		else
			attr[i] = NULL;
}

Lexem &Lexem::operator=(const Lexem &lexem)
{
	type = lexem.type;
	op_num = lexem.op_num;
	level = lexem.level;

	for (int i = 0; i < str_len; i++)
		str[i] = lexem.str[i];

	for (int i = 0; i < attr_cnt; i++)
		if (lexem.attr[i])
			attr[i] = &str[lexem.attr[i] - lexem.str];
		else
			attr[i] = NULL;

	return *this;
}

std::ostream & operator<<(std::ostream & os, Lexem &lexem)
{
	char **attr = lexem.get_attr();
	for (; attr && *attr; attr++)
		os << *attr << " ";
	return os;
}

bool Lexem::operator>(Lexem &lexem)
{
	return level > lexem.get_level();
}

bool Lexem::operator<(Lexem &lexem)
{
	return level < lexem.get_level();
}
bool Lexem::operator==(Lexem &lexem)
{
	return level == lexem.get_level();
}

void Lexem::clear_lexem()
{
	type = TYPE_NULL;
	op_num = OP_NULL;
	level = 0;

	for (int i = 0; i < str_len; i++)
		str[i] = '\0';

	attr[0] = str;
	for (int i = 1; i < attr_cnt; i++)
		attr[i] = NULL;
}

void Lexem::set_attr()
{
	char *s = str + 1;
	int attr_num = 1;
	int new_attr = 0;
	for (; s && *s && attr_num < attr_cnt; s++)
	{
		if (*s == '\0')
			break;
		else if (isspace(*s))
		{
			*s = '\0';
			new_attr = 1;
		}
		else
			if (new_attr)
			{
				attr[attr_num++] = s;
				new_attr = 0;
			}
	}
}

int Lexem::set_lexem_command(TYPE tp, OPNUM op, int lev, char ch, int ret)
{
	str[strlen(str)] = ch;
	str[strlen(str)] = '\0';
	type = tp;
	op_num = op;
	level = lev;
	if (ret == -1 || ret == 1)
		set_attr();

	return ret;
}

int Lexem::get_lexem(const char ch)
{
	int len = strlen(str);

	if (ch == '\0')
		return 0;

	if (len == 0)
	{
		if (isspace(ch))
			return 0;
		else if (ch == '(')
			return set_lexem_command(OPERATION, OP1, 1, ch, 1);
		else if (ch == ')')
			return set_lexem_command(OPERATION, OP2, 1, ch, 1);
		else if (ch == '<')
			return set_lexem_command(OPERATION, OP3, 2, ch, 1);
		else if (ch == '>')
			return set_lexem_command(OPERATION, OP4, 2, ch, 0);
		else if (ch == '|')
			return set_lexem_command(OPERATION, OP6, 3, ch, 0);
		else if (ch == '&')
			return set_lexem_command(OPERATION, OP8, 4, ch, 0);
		else
			return set_lexem_command(COMMAND, OP_NULL, 0, ch, 0);
	}
	else if (len == 1)
	{
		if (str[0] == '>')
			if (ch == '>')
				return set_lexem_command(OPERATION, OP5, 2, ch, 1);
			else
				return set_lexem_command(OPERATION, OP4, 2, '\0', -1);
		if (str[0] == '|')
			if (ch == '|')
				return set_lexem_command(OPERATION, OP7, 4, ch, 1);
			else
				return set_lexem_command(OPERATION, OP6, 3, '\0', -1);
		if (str[0] == '&')
			if (ch == '&')
				return set_lexem_command(OPERATION, OP9, 4, ch, 1);
			else
				return set_lexem_command(OPERATION, OP8, 4, '\0', -1);
		else
			if (ch == '(' || ch == ')' || ch == '<' || ch == '>'
						|| ch == '|' || ch == '&')
				return set_lexem_command(COMMAND, OP_NULL, 0, '\0', -1);
			else
				return set_lexem_command(COMMAND, OP_NULL, 0, ch, 0);
	}
	else
	{
		if (ch == '(' || ch == ')' || ch == '<' || ch == '>'
						|| ch == '|' || ch == '&')
			return set_lexem_command(COMMAND, OP_NULL, 0, '\0', -1);
		else
			return set_lexem_command(COMMAND, OP_NULL, 0, ch, 0);
	}

	return 0;
}
