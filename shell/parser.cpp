#include "lexem.h"
#include "parser.h"

void Parser::clear_stack()
{
	while (!v_stack.empty())
	{
		v_lex.push_back(v_stack.top());
		v_stack.pop();
	}
}

void Parser::add_lexem(Lexem lexem)
{
	if (lexem.get_type() == Lexem::TYPE_NULL)
		return;
	else if (lexem.get_type() == Lexem::COMMAND)
		v_lex.push_back(lexem);
	else
	{
		if (lexem.get_op_num() == Lexem::OP1)
			v_stack.push(lexem);
		else if (lexem.get_op_num() == Lexem::OP2)
		{
			if (v_stack.empty())
				return;
			Lexem l = v_stack.top();
			while (l.get_op_num() != Lexem::OP1)
			{
				v_lex.push_back(l);
				v_stack.pop();
				if (v_stack.empty())
					return;
				l = v_stack.top();
			}

			if (l.get_op_num() == Lexem::OP1)
				v_stack.pop();
		}
		else
		{
			if (v_stack.empty()
				|| v_stack.top().get_op_num() == Lexem::OP1
				|| v_stack.top() > lexem)
				v_stack.push(lexem);
			else
			{
				if (!v_stack.empty())
				{
					Lexem l = v_stack.top();
					while (!(l > lexem)
						&& l.get_op_num() != Lexem::OP1)
					{
						v_lex.push_back(l);
						v_stack.pop();
						if (v_stack.empty())
							break;
						l = v_stack.top();
					}
					if (l.get_op_num() == Lexem::OP1)
						v_stack.pop();
				}
				v_stack.push(lexem);

			}
		}
	}
}

