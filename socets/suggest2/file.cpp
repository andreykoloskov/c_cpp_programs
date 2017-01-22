#include "suggest.h"

int
read_file(std::ifstream &in, char buf0[], char ls[][WS])
{
	int count = 0;
	char bufd[BUFSIZE];
	bzero((char *) &bufd, sizeof(bufd));
	int flag;

	while (count < LS && strlen(buf0) && !in.eof()) {
		flag = 0;
		in.getline(bufd, WS - 2);
		if (!strlen(bufd) || bufd[0] == '\n' || bufd[0] == '\r')
			break;
		char *str = strstr(bufd, buf0);
		if (str && str == bufd) {
			strcat(bufd, "\n");
			for (int i = 0; i < count; i++) {
				char *str0 = strstr(ls[i], bufd);
				if (str0 && str0 == ls[i])
					if (strlen(str0) == strlen(ls[i])) {
						flag = 1;
						break;
					}
			}
			if (!flag) {
				strcat(ls[count], bufd);
				count++;
			}
		}
		bzero((char *) &bufd, sizeof(bufd));
	}
	return count;
}
