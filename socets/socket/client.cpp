#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

#define BUF 256

int main(int argc, char **argv)
{
	int sock;
	struct sockaddr_in addr;
	int conn;
	char buf[BUF];

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0)
		exit(1);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[1]));
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	conn = connect(sock, (struct sockaddr *)&addr, sizeof(addr));

	if (conn < 0)
		exit(2);

	for(;;) {
		bzero(buf, BUF);
		fgets(buf, BUF - 1, stdin);
		send(sock, buf, sizeof(buf), 0);
		bzero(buf, BUF);
		recv(sock, buf, sizeof(buf), 0);
		printf("%s\n", buf);
	}

	close(sock);

	return 0;
}
