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
#include <string>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <thread>
#include <list>
#include <mutex>
#include <map>

#define CNT 4
#define BUFSIZE 1024
#define WS 34
#define LS 10
#define CSZ 16

void
worker(std::list<int> *buf, std::mutex *m1);

int
read_file(std::ifstream &in, char buf0[], char ls[][WS]);

class Cache
{
	public:
		Cache();
		int read_cache(std::string &str0, std::string &str1);
		void write_cache(std::string str0, std::string str1);
		void aging_cache();
		void print_cache();
};

#endif
