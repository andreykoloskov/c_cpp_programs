#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF 256

int main(int argc, char **argv)
{
	int sock, listener;
	struct sockaddr_in addr;
	char buf[BUF];
	int bytes_read;
	int bnd;
	char str[] = "server:";

	listener = socket(AF_INET, SOCK_STREAM, 0);

	if (listener < 0)
		exit(1);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[1]));
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bnd = bind(listener, (struct sockaddr *)&addr, sizeof(addr));

	if (bnd < 0)
		exit(2);

	listen(listener, 1);

	for(;;) {
		sock = accept(listener, NULL, NULL);
		if (sock < 0)
			exit(3);

		for(;;) {
			bzero(buf, BUF);
			strcpy(buf, str);
			bytes_read = recv(sock, buf + strlen(str), BUF, 0);
			if (bytes_read <= 0)
				break;
			send(sock, buf, bytes_read, 0);
		}
		close(sock);
	}
	return 0;
}
