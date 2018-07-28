#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <stack>
#include "lexem.h"
#include "tree.h"

using std::vector;
using std::stack;

class Worker
{
private:
	int fd[2][2];
	int pipe_num;
	int pipe_max;
	int and_num;
	int and_max;
	int op_flag;
	void work_exec(Node *root);
	void work_exec_pipe(Node *root);
	void work_exec_and(Node *root);
	void work_op_3_4_5(Node *root, int op);
	void work_op_6(Node *root);
	void work_op_7_9(Node *root, int op);
	void pipe_cnt(Node *root);
	void and_cnt(Node *root);
	void work_root(Node *root);
public:
	Worker(Node *root);
	~Worker() {}
};

#endif
