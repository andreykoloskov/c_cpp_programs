#include "request.h"

int
main(int argc, char **argv)
{
	const char *opts = "d:h:p:w:";
	int opt;
	char path[PATH_MAX];
	char ip[PATH_MAX];
	int port;
	int sv[2];
	int cnt;

	if(argc == 1)
		return 0;

	if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sv) < 0)
		return 0;

	while((opt = getopt(argc, argv, opts)) != -1) {
		switch(opt) {
			case 'd':
				strcpy(path, optarg);
				break;
			case 'h':
				strcpy(ip, optarg);
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case 'w':
				cnt = atoi(optarg);
			default:
				break;
		}
	}

	for (int i = 0; i < cnt; i++) {
		if (!fork()) {
			close(sv[0]);
			worker(sv[1], path);
		} else {
			if (i == cnt - 1) {
				close(sv[1]);
				master(sv[0], ip, port);
			}
		}
	}

	return 0;
}
