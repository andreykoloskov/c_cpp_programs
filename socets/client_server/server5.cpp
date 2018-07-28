#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/epoll.h>

using namespace std;





int main() {


    int sockid = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr, cli_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    unsigned short portnum = 3900;
    serv_addr.sin_port = htons(portnum);
    bind(sockid, (sockaddr *) &serv_addr, sizeof(serv_addr));
    listen(sockid, 5);
    char buffer[256];
    bzero(buffer, 256);



    int sockid2 = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr2, cli_addr2;
    serv_addr2.sin_family = AF_INET;
    serv_addr2.sin_addr.s_addr = INADDR_ANY;
    unsigned short portnum2 = 3910;
    serv_addr2.sin_port = htons(portnum2);
    bind(sockid2, (sockaddr *) &serv_addr2, sizeof(serv_addr2));
    listen(sockid2, 5);
    socklen_t clilen = sizeof(cli_addr2);

    int epfd = epoll_create(1);


    static struct epoll_event* events = (epoll_event*)calloc(64, sizeof(struct epoll_event));

    static struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sockid;

    static struct epoll_event ev2;
    ev2.events = EPOLLIN;
    ev2.data.fd = sockid2;

    epoll_ctl(epfd, EPOLL_CTL_ADD, sockid, &ev);
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockid2, &ev2);
    while(1) {
        int nfds = epoll_wait(epfd, events,
                10,
                -1);

        cout << "Events in query: " << nfds << endl;
        for(int i = 0; i < nfds; i++) {
            int fd = events[i].data.fd;
            int newsockid = accept(sockid, (sockaddr *) &cli_addr, &clilen);
            recv(newsockid, buffer, 256, 0);
            read(newsockid, buffer, 256);
            cout << buffer;
            close(newsockid);
        }
    }
    return 0;
}
