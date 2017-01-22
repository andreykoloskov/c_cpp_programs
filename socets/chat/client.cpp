#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUF 256

/*
При запуске данного клиента передаются 2 аргумента:
127.0.0.1 и сокет сервера
*/

int main(int argc, char *argv[])
{
	int sock, n;
	struct sockaddr_in saddr;
	struct hostent *server;
	char buf[BUF];
	char *addr;

	if (argc < 3)
		return 1;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cout << "ERROR opening socket" << std::endl;
		return 1;
	}

	if ((server = gethostbyname(argv[1])) == NULL) {
		std::cout << "ERROR, no such host" << std::endl;
		return 1;
	}

	bzero((char *) &saddr, sizeof(saddr));
	addr = (char *) &saddr.sin_addr.s_addr;
	saddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, addr, server->h_length);
	saddr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr *) &saddr, sizeof(saddr)) < 0)
		std::cout << "ERROR connecting" << std::endl;

	for(;;) {
		bzero(buf, BUF);
		fgets(buf, BUF - 1, stdin);

		if ((n = send(sock, buf, strlen(buf), MSG_NOSIGNAL)) < 0)
			std::cout << "ERROR writing" << std::endl;

		bzero(buf, BUF);
		if ((n = recv(sock, buf, BUF - 1, MSG_NOSIGNAL)) < 0)
			std::cout << "ERROR reading" << std::endl;
		std::cout << buf;
		std::cout.flush();
		if (buf[0] == '0')
			break;
	}

	shutdown(sock, SHUT_RDWR);
	close(sock);
	return 0;
}
