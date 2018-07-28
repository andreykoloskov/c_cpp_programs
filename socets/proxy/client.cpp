#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUF 256

//При запуске данного клиента передаются 2 аргумента: 127.0.0.1 и сокет прокси сервера

int main(int argc, char *argv[])
{
	if (argc < 3) {
		std::cout << "Usage " << argv[0] << "hostname port" << std::endl;
		return 1;
	}

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[BUF];

    portno = atoi(argv[2]);
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    	std::cout << "ERROR opening socket" << std::endl;
    	return 1;
	}

    if ((server = gethostbyname(argv[1])) == NULL) {
    	std::cout << "ERROR, no such host" << std::endl;
    	return 1;
	}

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    	std::cout << "ERROR connecting" << std::endl;

	std::cout << "Please enter the message: ";
    bzero(buffer, BUF);
    fgets(buffer, BUF - 1, stdin);
	if ((n = send(sockfd, buffer, strlen(buffer), MSG_NOSIGNAL)) < 0)
		std::cout << "ERROR writing to socket" << std::endl;

    bzero(buffer, BUF);
    if ((n = recv(sockfd, buffer, BUF - 1, MSG_NOSIGNAL)) < 0)
    	std::cout << "ERROR reading from socket" << std::endl;
	std::cout << buffer << std::endl;

    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);
    return 0;
}
