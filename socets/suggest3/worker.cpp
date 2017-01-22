#include "suggest.h"

void
worker(int sock, char *path)
{
	int fd = -1;
	char buf[16];
	ssize_t size;
	static char Buffer[BUFSIZE];
	static char Buffer2[BUFSIZE2];
	std::cout << "worker_pid = " << getpid() << std::endl;
	for (;;) {
		size = sock_fd_read(sock, buf, 1, &fd);
		if (fd != -1) {
			std::cout << "current_worker_pid = " << getpid() << std::endl;
			bzero((char *) &Buffer, sizeof(Buffer));
			bzero((char *) &Buffer2, sizeof(Buffer2));
			char method[10];
			char file_name[PATH_MAX];

			int RecvSize = recv(fd, Buffer, BUFSIZE, MSG_NOSIGNAL);

			sscanf(Buffer, "%s %s", method, file_name);
			if (!is_method(method)) {
				request(method, path, file_name, Buffer2);
				send(fd, Buffer2, strlen(Buffer2), MSG_NOSIGNAL);
			}
			close(fd);
		}
		sleep(5);
	}
}
