#include <iostream>
#include <algorithm>
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

#include <netdb.h>

#define MAX_EVENTS 32
#define BUFSIZE 1024

//При запуске данного прокси сервера передаются 2 аргумента: собственный сокет и сокет конечного сервера

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
	if (argc < 3) {
		std::cout << "Little arguments" << std::endl;
		return 1;
	}

	int MasterSocket;
	int Result;

	if ((MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		std::cout << strerror(errno) << std::endl;
		return 1;
	}

	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(atoi(argv[1]));
	SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((Result = bind(MasterSocket, (struct sockaddr *)&SockAddr, sizeof(SockAddr))) == -1) {
		std::cout << strerror(errno) << std::endl;
		return 1;
	}

	set_nonblock(MasterSocket);

	if ((Result = listen(MasterSocket, SOMAXCONN)) == -1) {
		std::cout << strerror(errno) << std::endl;
		return 1;
	}

	//Функционал клиента для отправки сообщений на другой сервер
	int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    portno = atoi(argv[2]);
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    	std::cout << "ERROR opening socket" << std::endl;
    	return 1;
	}

    if ((server = gethostbyname("127.0.0.1")) == NULL) {
    	std::cout << "ERROR, no such host" << std::endl;
    	return 1;
	}

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    	std::cout << "ERROR connecting" << std::endl;


	//////////////////////////////////////////////////////////////


	struct epoll_event Event;
	Event.data.fd = MasterSocket;
	Event.events = EPOLLIN | EPOLLET;

	struct epoll_event * Events;
	Events = (struct epoll_event *) calloc(MAX_EVENTS, sizeof(struct epoll_event));

	int EPoll = epoll_create1(0);
	epoll_ctl(EPoll, EPOLL_CTL_ADD, MasterSocket, &Event);

	static char Buffer[BUFSIZE];
	int N;

	while(true)
	{
		N = epoll_wait(EPoll, Events, MAX_EVENTS, -1);

		for(unsigned int i = 0; i < N; i++)
		{
			if((Events[i].events & EPOLLERR)||(Events[i].events & EPOLLHUP))
			{
				shutdown(Events[i].data.fd, SHUT_RDWR);
				close(Events[i].data.fd);
			}
			else if(Events[i].data.fd == MasterSocket)
			{
				int SlaveSocket = accept(MasterSocket, 0, 0);
				set_nonblock(SlaveSocket);

				struct epoll_event Event;
				Event.data.fd = SlaveSocket;
				Event.events = EPOLLIN | EPOLLET;

				epoll_ctl(EPoll, EPOLL_CTL_ADD, SlaveSocket, &Event);
			}
			else
			{
				int RecvSize = recv(Events[i].data.fd, Buffer, BUFSIZE, MSG_NOSIGNAL);
				send(sockfd, Buffer, RecvSize, MSG_NOSIGNAL);
				send(Events[i].data.fd, Buffer, RecvSize, MSG_NOSIGNAL);
			}
			if (Buffer[0] == '0') break;
		}
		if (Buffer[0] == '0') break;
	}

	for(unsigned int i = 0; i < N; i++) {
		shutdown(Events[i].data.fd, SHUT_RDWR);
		close(Events[i].data.fd);
	}
	shutdown(sockfd, SHUT_RDWR);
	close(sockfd);

	return 0;
}
