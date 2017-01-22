#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/epoll.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_EVENTS 32
#define BUFSIZE 1024

struct S
{
	ssize_t sock;
	struct in_addr IP;
};

//При запуске данного сервера передаются 1 аргумент: порт
int set_nonblock(int fd)
{
	int flags;
#if defined(O_NONBLOCK)
	if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
		flags = 0;
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
	flags = 1;
	return ioctl(fd, FIOBIO, &flags);
#endif
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cout << "Little arguments" << std::endl;
		return 1;
	}

	int master;
	int res;

	if ((master = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		std::cout << strerror(errno) << std::endl;
		return 1;
	}

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(argv[1]));
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	res = bind(master, (struct sockaddr *)&saddr, sizeof(saddr));

	if (res == -1)
		return 1;

	set_nonblock(master);

	res = listen(master, SOMAXCONN);

	if (res == -1)
		return 1;

	struct epoll_event event;
	event.data.fd = master;
	event.events = EPOLLIN | EPOLLET;
	struct epoll_event * events;

	events = (struct epoll_event *) calloc(MAX_EVENTS, sizeof(*events));

	int EPoll = epoll_create1(0);
	epoll_ctl(EPoll, EPOLL_CTL_ADD, master, &event);

	static char Buffer[BUFSIZE];
	int N;
	int RecvSize;

	socklen_t clilen;
	struct sockaddr_in cli_addr;
	clilen = sizeof(cli_addr);
	std::vector<struct S> slaves;

	while(true)
	{
		N = epoll_wait(EPoll, events, MAX_EVENTS, -1);

		for(unsigned int i = 0; i < N; i++)
		{
			if((events[i].events & EPOLLERR)||(events[i].events & EPOLLHUP))
			{
				for (std::vector<struct S>::iterator p = slaves.begin(); p != slaves.end(); p++)
					if (p->sock == events[i].data.fd) {
						slaves.erase(p);
						break;
					}

				shutdown(events[i].data.fd, SHUT_RDWR);
				close(events[i].data.fd);
			}
			else if(events[i].data.fd == master)
			{
				int SlaveSocket = accept(master, (struct sockaddr *) &cli_addr, &clilen);
				set_nonblock(SlaveSocket);
				//Добавляем слейвы в массив
				S s = {SlaveSocket, cli_addr.sin_addr};
				slaves.push_back(s);

				struct epoll_event event;
				event.data.fd = SlaveSocket;
				event.events = EPOLLIN | EPOLLET;

				epoll_ctl(EPoll, EPOLL_CTL_ADD, SlaveSocket, &event);
			}
			else
			{
				int RecvSize = recv(events[i].data.fd, Buffer, BUFSIZE, MSG_NOSIGNAL);
				int RecvSize2;

				for (std::vector<struct S>::iterator p = slaves.begin(); p != slaves.end(); p++) {
					if (events[i].data.fd == p->sock) {
						const char bf = '\0';
						send(p->sock, &bf, 0, MSG_NOSIGNAL);
					} else {
						if (RecvSize > 1) {
							send(p->sock, "IP: ", strlen("IP: "), MSG_NOSIGNAL);
							send(p->sock, inet_ntoa(p->IP), strlen(inet_ntoa(p->IP)), MSG_NOSIGNAL);
							send(p->sock, "; ", strlen("; "), MSG_NOSIGNAL);
							send(p->sock, "msg: ", strlen("msg: "), MSG_NOSIGNAL);
						}
						send(p->sock, Buffer, RecvSize, MSG_NOSIGNAL);
					}
				}
			}
		}
	}

	for(unsigned int i = 0; i < N; i++) {
		shutdown(events[i].data.fd, SHUT_RDWR);
		close(events[i].data.fd);
	}

	return 0;
}
