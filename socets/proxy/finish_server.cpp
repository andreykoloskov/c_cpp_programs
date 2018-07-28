#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define BUF 256
#define MAXCOUNT 5

//При запуске данного конечного сервера передается 1 аргумент: собственный сокет

int main(int argc, char *argv[])
{
	if (argc < 2) { fprintf(stderr,"ERROR, no port provided\n"); exit(0); }

	int sockfd, newsockfd, portno, n;
	socklen_t clilen;
	char buffer[BUF], buffer2[BUF];
	struct sockaddr_in serv_addr, cli_addr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { fprintf(stderr, "ERROR opening socket"); exit(0); }

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) fprintf(stderr, "ERROR on binding");
	listen(sockfd, MAXCOUNT);
	clilen = sizeof(cli_addr);

	if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) < 0) fprintf(stderr, "ERROR on accept");
	while (1) {
		bzero(buffer, BUF);

		if ((n = recv(newsockfd, buffer, BUF - 1, MSG_NOSIGNAL)) < 0) fprintf(stderr, "ERROR reading from socket");
		printf("%s\n", buffer);

		if (buffer[0] == '0')
			break;
	}


	shutdown(sockfd, SHUT_RDWR);
	close(sockfd);
	return 0;
}
