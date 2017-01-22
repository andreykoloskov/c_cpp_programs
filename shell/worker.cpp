#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "lexem.h"
#include "tree.h"
#include "worker.h"

void Worker::work_exec(Node *root)
{
	Lexem lex = root->lexem;

	if (fork() == 0)
		if (execvp(lex.get_attr()[0], lex.get_attr()) < 0)
			exit(1);

	wait(NULL);
}

void Worker::work_exec_pipe(Node *root)
{
	Lexem lex = root->lexem;

	pipe(fd[pipe_num % 2]);
	if (fd[(pipe_num + 1) % 2][1])
		close(fd[(pipe_num + 1) % 2][1]);

	if (fork() == 0)
	{
		if (pipe_max)
		{
			if (fd[(pipe_num + 1) % 2][0])
			{
				dup2(fd[(pipe_num + 1) % 2][0], 0);
				close(fd[(pipe_num + 1) % 2][0]);
			}

			if (fd[(pipe_num + 1) % 2][1])
				close(fd[(pipe_num + 1) % 2][1]);

			if (pipe_num < pipe_max)
				dup2(fd[pipe_num % 2][1], 1);

			if (fd[pipe_num % 2][0])
				close(fd[pipe_num % 2][0]);
			if (fd[pipe_num % 2][1])
				close(fd[pipe_num % 2][1]);
		}

		if (execvp(lex.get_attr()[0], lex.get_attr()) < 0)
			exit(1);
	}

	if (pipe_max)
		if (fd[(pipe_num + 1) % 2][0])
			close(fd[(pipe_num + 1) % 2][0]);

	if (!pipe_max || (pipe_num == pipe_max && and_num == and_max))
	{
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				if (fd[i][j])
					close(fd[i][j]);

		while (wait(NULL) != -1);
	}

	pipe_num++;
}

void Worker::work_exec_and(Node *root)
{
	Lexem lex = root->lexem;

	if (fork() == 0)
	{
		if (fd[pipe_num % 2][0])
			close(fd[pipe_num % 2][0]);
		if (pipe_num < pipe_max)
		{
			dup2(fd[pipe_num % 2][1], 1);
			if (fd[pipe_num % 2][1])
				close(fd[pipe_num % 2][1]);
		}

		if (execvp(lex.get_attr()[0], lex.get_attr()) < 0)
			exit(1);
	}
	and_num++;
}

void Worker::work_op_3_4_5(Node *root, int op)
{
	int f;

	if (!root->left || !root->right)
		exit(1);

	Lexem l = root->left->lexem;
	Lexem file = root->right->lexem;

	if (op == 3)
		f = open(file.get_attr()[0], O_RDONLY);
	else if (op == 4)
		f = open(file.get_attr()[0], O_WRONLY | O_TRUNC | O_CREAT, 0777);
	else if (op == 5)
		f = open(file.get_attr()[0], O_WRONLY | O_APPEND | O_CREAT, 0777);

	if (f != -1)
	{
		if (op == 3)
			dup2(f, 0);
		else
			dup2(f, 1);
		close(f);
	}

	work_root(root->left);
}

void Worker::work_op_6(Node *root)
{
	if (!root->left || !root->right)
		exit(1);

	op_flag = 1;
	work_root(root->left);
	op_flag = 1;
	work_root(root->right);
}

void Worker::work_op_7_9(Node *root, int op)
{
	int st1;

	if (!root->left || !root->right)
		exit(1);

	if ((root->left->lexem.get_type() == Lexem::COMMAND ||
            root->left->lexem.get_op_num() == Lexem::OP3 ||
            root->left->lexem.get_op_num() == Lexem::OP4 ||
            root->left->lexem.get_op_num() == Lexem::OP5) &&
            pipe_num == 0)
		pipe(fd[pipe_num % 2]);

	if (pipe_num == 0)
		op_flag = 2;

	work_root(root->left);

	wait(&st1);

	int res = 0;

	if (op == 7 && (!WIFEXITED(st1) || WEXITSTATUS(st1) != 0))
		res = 1;
	else if (op == 9 && (WIFEXITED(st1) && WEXITSTATUS(st1) == 0))
		res = 1;

	if (res)
	{
		op_flag = 2;
		work_root(root->right);
		if (and_num == and_max + 1)
		{
			if (pipe_num == pipe_max + 1)
			{
				close(fd[pipe_num % 2][0]);
				close(fd[pipe_num % 2][1]);
			}
			else
				pipe_num++;

			wait(NULL);
		}
	}
}

void Worker::work_root(Node *root)
{
	if (!root)
		return;

	Lexem lex = root->lexem;

	if (lex.get_type() == Lexem::TYPE_NULL)
		return;
	else if (lex.get_type() == Lexem::COMMAND)
	{
		if (op_flag == 0)
			work_exec(root);
		else if (op_flag == 1)
			work_exec_pipe(root);
		else if (op_flag == 2)
			work_exec_and(root);
	}
	else if (lex.get_type() == Lexem::OPERATION)
	{
		if (lex.get_op_num() == Lexem::OP3)
			work_op_3_4_5(root, 3);
		else if (lex.get_op_num() == Lexem::OP4)
			work_op_3_4_5(root, 4);
		else if (lex.get_op_num() == Lexem::OP5)
			work_op_3_4_5(root, 5);
		else if (lex.get_op_num() == Lexem::OP6)
			work_op_6(root);
		else if (lex.get_op_num() == Lexem::OP7)
			work_op_7_9(root, 7);
		else if (lex.get_op_num() == Lexem::OP8)
		{
			if (fork() == 0)
				work_root(root->left);
			wait(NULL);
		}
		else if (lex.get_op_num() == Lexem::OP9)
			work_op_7_9(root, 9);
	}
}

void Worker::pipe_cnt(Node *root)
{
	if (root->left)
		pipe_cnt(root->left);
	if (root->right)
		pipe_cnt(root->right);
	if (root->lexem.get_op_num() == Lexem::OP6)
		pipe_max++;
}

void Worker::and_cnt(Node *root)
{
	if (root->left)
		and_cnt(root->left);
	if (root->right)
		and_cnt(root->right);
	if (root->lexem.get_op_num() == Lexem::OP9)
		and_max++;
}

Worker::Worker(Node *root)
{
	pipe_num = 0;
	pipe_max = 0;
	and_num = 0;
	and_max = 0;
	op_flag = 0;
	pipe_cnt(root);
	and_cnt(root);
	work_root(root);
}

