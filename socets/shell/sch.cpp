#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 256
#define MAX2 1024 * 128

//Структура элемента строки
struct Elem
{
	char *param[MAX];
	int count;
	int ch1;
	int ch2;
	char *fl;
};

//Обработчик сигнала
void handler(int signo)
{
    int status = 0;
    int pid = wait(&status);
    std::cout << "PID = " << pid << " exit = " << status << std::endl;
    signal(SIGCHLD, handler);
}

//Парсер строки
void parse(std::vector<struct Elem> & el, char *str)
{
	char *p = str;
	struct Elem elem;
	int k;

	while (*p) {
		k = 0;
		elem.param[k] = NULL;
		elem.count = 0;
		elem.ch1 = 0;
		elem.ch2 = 0;
		elem.fl = NULL;

		while (*p && *p != '|') {
			if (*p != ' ') {
				if (k == 0) {
					elem.param[k] = p;
					while (*p && *p != ' ')
						p++;
					k++;
				} else {
					if (*p == '&') {
						elem.ch1 = 1;
					} else if (*p == '>') {
						elem.ch2 = 1;
					} else {
						if (elem.ch2) {
							elem.fl = p;
							while (*p && *p != ' ')
								p++;
						}
						if (!elem.ch1 && !elem.ch2) {
							elem.param[k] = p;
							while (*p && *p != ' ')
								p++;
							k++;
						}
					}
				}
			}

			elem.count = k;
			elem.param[k] = NULL;

			if (*p && *p == ' ')
				*p = '\0';

			p++;
		}

		el.push_back(elem);

		if (*p && *p == ' ')
			*p = '\0';

		p++;
	}
}

//Отладочная печать
void print_v(std::vector<struct Elem> & el)
{
	for (std::vector<struct Elem>::iterator p = el.begin(); p < el.end(); p++) {
		std::cout << p->count << std::endl;
		for (int i = 0; i < p->count; i++)
			std::cout << p->param[i] << " ";
		std::cout << std::endl;

		if (p->ch1)
			std::cout << "&" << std::endl;
		if (p->ch2) {
			std::cout << ">" << std::endl;
			std::cout << p->fl << std::endl;
		}

		if (p + 1 < el.end())
			std::cout << "|" << std::endl;

	}
}

int main(int argc, char **argv)
{
	char str[MAX2];
	std::vector<struct Elem> el;
	int fd;
	int fd1[2], fd2[2];
	int pid;
	int k;

	signal(SIGCHLD, handler);
	for (;;) {
		for (int i = 0; i < MAX2; i++)
			str[i] = 0;
		std::cin.getline(str, MAX2 - 1);
		parse(el, str);
		std::vector<struct Elem>::iterator p;
		//Без конвеера
		if (el.size() == 1) {
			p = el.begin();
			fd = -1;
			if (p->fl)
				fd = open(p->fl, O_WRONLY | O_CREAT, 0664);
			pid = fork();
			if (!pid) {
				std::cout << "PID = " << getpid() << " " << p->param[0] << std::endl;
				if (fd != -1) {
					close(1);
					dup2(fd, 1);
				}
				execvp(p->param[0], p->param);
				//Без '&'
				if (!p->ch1) pause();
			}
			if (fd != -1)
				close(fd);

		//С конвеером
		} else {
			//Создание конвеера
			//1 - й элемент
			k = 0;
			p = el.begin();

			fd = -1;
			if (p->fl)
				fd = open(p->fl, O_WRONLY | O_CREAT, 0664);

			pipe(fd1);
			pid = fork();
			if (!pid) {
				std::cout << "PID = " << getpid() << " " << p->param[0] << std::endl;
				if (fd != -1) {
					close(1);
					dup2(fd, 1);
				} else {
					dup2(fd1[1], 1);
				}
				close(fd1[0]); close(fd1[1]);
				execvp(p->param[0], p->param);
			}
			if (fd != -1)
				close(fd);
			k++; p++;

			//Внутренние элементы
			for (; p < el.end() - 1; p++, k++) {
				if (k % 2) {
					if (fd2[0]) close(fd2[0]);
					if (fd2[1]) close(fd2[1]);
					//Без '&'
					if (!p->ch1) pause();

					fd = -1;
					if (p->fl)
						fd = open(p->fl, O_WRONLY | O_CREAT, 0664);

					pipe(fd2);
					pid = fork();
					if (!pid) {
						std::cout << "PID = " << getpid() << " " << p->param[0] << std::endl;
						dup2(fd1[0], 0);
						close(fd1[0]); close(fd1[1]);

						if (fd != -1) {
							close(1);
							dup2(fd, 1);
						} else {
							dup2(fd2[1], 1);
						}
						close(fd2[0]); close(fd2[1]);
						execvp(p->param[0], p->param);
					}
					if (fd != -1)
						close(fd);
				} else {
					if (fd1[0]) close(fd1[0]);
					if (fd1[1]) close(fd1[1]);
					//Без '&'
					if (!p->ch1) pause();

					fd = -1;
					if (p->fl)
						fd = open(p->fl, O_WRONLY | O_CREAT, 0664);

					pipe(fd1);
					pid = fork();
					if (!pid) {
						std::cout << "PID = " << getpid() << " " << p->param[0] << std::endl;
						dup2(fd2[0], 0);
						close(fd2[0]); close(fd2[1]);

						if (fd != -1) {
							close(1);
							dup2(fd, 1);
						} else {
							dup2(fd1[1], 1);
						}
						close(fd1[0]); close(fd1[1]);
						execvp(p->param[0], p->param);
					}
					if (fd != -1)
						close(fd);
				}
			}

			//Последний элемент
			if (k % 2) {
				if (fd2[0]) close(fd2[0]);
				if (fd2[1]) close(fd2[1]);
			} else {
				if (fd1[0]) close(fd1[0]);
				if (fd1[1]) close(fd1[1]);
			}
			//Без '&'
			if (!p->ch1) pause();

			fd = -1;
			if (p->fl)
				fd = open(p->fl, O_WRONLY | O_CREAT, 0664);

			pid = fork();
			if (!pid) {
				std::cout << "PID = " << getpid() << " " << p->param[0] << std::endl;
				if (k % 2) {
					dup2(fd1[0], 0);
					close(fd1[0]); close(fd1[1]);
				} else {
					dup2(fd2[0], 0);
					close(fd2[0]); close(fd2[1]);
				}

				if (fd != -1) {
					close(1);
					dup2(fd, 1);
				}

				execvp(p->param[0], p->param);
			}
			if (fd != -1)
				close(fd);
			k++; p++;

			if (k % 2) {
				close(fd2[0]); close(fd2[1]);
			} else {
				close(fd1[0]); close(fd1[1]);
			}
		}
		el.erase(el.begin(), el.end());
	}
	return 0;
}
