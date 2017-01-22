#include "suggest.h"

key_t key;
int semid, shmid;
struct sembuf sops;
char *shmaddr;

int
main(int argc, char **argv)
{
	if (argc < 2) {
		std::cout << "Little arguments" << std::endl;
		return 1;
	}

	int sv[2];

	if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sv) < 0) {
		perror("socketpair");
		exit(1);
	}

	key = ftok(FTOK_FILE, FTOK_PROJ);

	int csize = (CSZ + 1) * sizeof(struct Cache);
	if ((shmid = shmget(key, csize, IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		return 1;
	}
	if ((shmaddr = (char *) shmat(shmid, NULL, 0)) == (void *) -1) {
		perror("shmat");
		return 1;
	}

	//Флаг свободного кэша
	//Последний элемент - для хранения настроек
	for (int i = 0; i <= CSZ; i++) {
		struct Cache *addr = &(((struct Cache *) shmaddr)[i]);
		addr->busy = 0;
	}

	semid = semget(key, 1, IPC_CREAT | 0666);
	semctl(semid, 0, IPC_SET, 0);
	sops.sem_num = 0;
	sops.sem_flg = 0;
	sops.sem_op = 1;
	semop(semid, &sops, 1);

	for (int i = 0; i < CNT; i++) {
		if (!fork()) {
			close(sv[0]);
			worker(sv[1]);
		} else {
			if (i == CNT - 1) {
				close(sv[1]);
				int port = atoi(argv[1]);
				master(sv[0], port);
			}
		}
	}

	shmdt(shmaddr);
	semctl(semid, 0, IPC_RMID, 0);
	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}
