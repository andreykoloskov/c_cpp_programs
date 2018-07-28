#ifndef SUGGEST_HEADER
#define SUGGEST_HEADER

#include <iostream>
#include <fstream>
#include <cstdlib>
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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define CNT 4
#define BUFSIZE 1024
#define WS 34
#define LS 10
#define CSZ 16
#define FTOK_FILE "file123"
#define FTOK_PROJ 0

struct Cache
{
	int busy;
	char word[WS];
};

ssize_t
sock_fd_write(int sock, void *buf, ssize_t buflen, int fd);

ssize_t
sock_fd_read(int sock, void *buf, ssize_t bufsize, int *fd);

void
master(int sock, int port);

void
worker(int sock);

int
read_file(std::ifstream &in, char buf0[], char ls[][WS], int cnt0);

int
read_cache(char buf0[], char ls[][WS]);

void
write_cache(char ls[][WS], int cnt0, int cnt1);

void
print_cache(void);

#endif
