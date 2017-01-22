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
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define BUFSIZE 1024
#define BUFSIZE2 1024 * 128
#define BUFSIZE3 1024 * 128

void
request(char method[], char path[], char file_name[], char Buffer2[]);

int
is_method(char method[]);

ssize_t
sock_fd_write(int sock, void *buf, ssize_t buflen, int fd);

ssize_t
sock_fd_read(int sock, void *buf, ssize_t bufsize, int *fd);

void
master(int sock, char *ip, int port);

void
worker(int sock, char *path);

#endif
