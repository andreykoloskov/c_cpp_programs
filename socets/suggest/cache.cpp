#include "suggest.h"

extern char *shmaddr;

int
read_cache(char buf0[], char ls[][WS])
{
	int count = 0;
	if (!strlen(buf0))
		return count;
	for (int i = 0; i < CSZ; i++) {
		struct Cache *addr = &(((struct Cache *) shmaddr)[i]);
		if (addr->busy) {
			char *str = strstr(addr->word, buf0);
			if (str && str == addr->word) {
				strcat(ls[count], addr->word);
				count++;
			}
		}
		if (count == LS)
			break;
	}
	return count;
}

void
write_cache(char ls[][WS], int cnt0, int cnt1)
{
	//Последний элемент кэша для хранения настроек
	struct Cache *addr_end = &(((struct Cache *) shmaddr)[CSZ]);
	int cpt = addr_end->busy;

	for (int i = cnt0; i < cnt1; i++) {
		struct Cache *addr = &(((struct Cache *) shmaddr)[cpt]);
		addr->busy = 1;
		strcpy(addr->word, ls[i]);
		cpt = (cpt + 1) % CSZ;
		addr_end->busy = cpt;
	}
}

void
print_cache(void)
{
	std::cout << "Cache:" << std::endl;
	for (int i = 0; i < CSZ; i++) {
		struct Cache *addr = &(((struct Cache *) shmaddr)[i]);
		if (addr->busy)
			std::cout << i << ": " <<  addr->word;
	}
}
