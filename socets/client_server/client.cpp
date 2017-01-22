#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <netdb.h>
#include <string.h>

#include <fcntl.h>

using namespace std;

int main() {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    portno = 490;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    char buffer[256];
    bzero(buffer,256);

 
   connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    fgets(buffer, 255, stdin);
    send(sockfd, buffer,256, 0);
    close(sockfd);


  return 0;
}
