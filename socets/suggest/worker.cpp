#include "suggest.h"

extern key_t key;
extern int semid, shmid;
extern struct sembuf sops;
extern char *shmaddr;

void
worker(int sock)
{
	int fd = -1;
	char buf[16];
	ssize_t size;
	char buf0[BUFSIZE];
	char buf1[BUFSIZE];
	char ls[LS][WS];
	int cnt0;
	int cnt1;
	const char file_name[] = "list.txt";
	int rsz;
	int k;
	std::ifstream in(file_name, std::ios::in | std::ios::binary);
	for (;;) {
		size = sock_fd_read(sock, buf, 1, &fd);
		if (fd != -1) {
			std::cout << "worker_pid = " << getpid() << std::endl;
			bzero((char *) &buf0, sizeof(buf0));
			bzero((char *) &buf1, sizeof(buf1));
			for (int i = 0; i < LS; i++)
				bzero((char *) ls[i], sizeof(ls[i]));

			rsz = recv(fd, buf0, BUFSIZE, MSG_NOSIGNAL);
			k = strlen(buf0) - 1;
			while(k >= 0 && (buf0[k] == 10 || buf0[k] == 13)) {
				buf0[k] = '\0';
				k = strlen(buf0) - 1;
			}

			sops.sem_op = -1;
			semop(semid, &sops, 1);
			cnt0 = read_cache(buf0, ls);
			sops.sem_op = 1;
			semop(semid, &sops, 1);

			if (cnt0 < LS) {
				cnt1 = read_file(in, buf0, ls, cnt0);

				sops.sem_op = -1;
				semop(semid, &sops, 1);
				write_cache(ls, cnt0, cnt1);
				sops.sem_op = 1;
				semop(semid, &sops, 1);
			} else
				cnt1 = cnt0;

			for (int i = 0; i < cnt1; i++) {
				strcat(buf1, ls[i]);
			}

			rsz = strlen(buf1);
			if (!rsz) {
				strcpy(buf1, "\n");
				rsz = 1;
			}

			send(fd, buf1, rsz, MSG_NOSIGNAL);
			close(fd);
		}
		in.seekg(0, std::ios::beg);
		sleep(5);
	}
	in.close();
}
