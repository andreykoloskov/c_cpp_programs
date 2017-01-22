#include "suggest.h"

void
worker(std::list<int> *lst, std::mutex *m1)
{
	int fd = -1;
	int cnt1;
	char buf0[BUFSIZE];
	char buf1[BUFSIZE];
	char ls[LS][WS];
	const char file_name[] = "list.txt";
	int rsz;
	int k;
	std::ifstream in(file_name, std::ios::in | std::ios::binary);
	std::string str0;
	std::string str1;
	Cache cache;
	std::mutex mut;

	while (true) {
		m1->lock();
		//Проверка не пустоты
		if (lst->size()) {
			fd = lst->front();
			lst->pop_front();
			m1->unlock();
			if (fd != -1) {
				std::cout << "thread_pid = " << std::this_thread::get_id() << std::endl;
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

				str0 = buf0;
				if (!cache.read_cache(str0, str1)) {
					cnt1 = read_file(in, buf0, ls);
					for (int i = 0; i < cnt1; i++) {
						strcat(buf1, ls[i]);
					}
					rsz = strlen(buf1);
					str1 = buf1;
					if (rsz) {
						mut.lock();
						cache.write_cache(str0, str1);
						mut.unlock();
					} else
						strcpy(buf1, "\n");
				}
				const char *buf2 = str1.c_str();
				rsz = strlen(buf2);
				send(fd, buf2, rsz, MSG_NOSIGNAL);
				close(fd);
			}

		} else {
			m1->unlock();
			sleep(1);
		}
		in.seekg(0, std::ios::beg);
	}
	in.close();
}
