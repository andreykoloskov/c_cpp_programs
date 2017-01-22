#include "suggest.h"

void request(char method[], char path[], char file_name[], char Buffer2[])
{
	char path1[PATH_MAX];
	char path2[PATH_MAX];
	char buf2[32];
	strcpy(path1, path);
	unsigned char buf[BUFSIZE];
	static char Buffer3[BUFSIZE3];
	int count;

	//Check path
	strcpy(path2, file_name);
	char *p = strrchr(path2, '/');
	if (p) {
		if (path2[0] == '/') {
			file_name += 1;
		}
		*p = '\0';

		if (path1[strlen(path) - 1] == '/')
			strcat(path1, path2);
		else {
			strcat(path1, "/");
			strcat(path1, path2);
		}

		chdir(path1);
		getcwd(path2, PATH_MAX);
		chdir(path);
		getcwd(path1, PATH_MAX);

		if (!strstr(path2, path1)) {
			strcpy(Buffer2, "HTTP/1.0 404 Not Found\n\n");
			return;
		}
	} else {
		chdir(path);
	}

	//Open file
	std::ifstream in(file_name, std::ios::in | std::ios::binary);

	if (!in) {
		strcpy(Buffer2, "HTTP/1.0 404 Not Found\n\n");
		return;
	}

	//Work http request
	if (!strcmp(method, "HEAD"))
		strcpy(Buffer2, "HTTP/1.0 200 OK\n\n");
	else if (!strcmp(method, "GET") || !strcmp(method, "POST")) {
		strcpy(Buffer2, "HTTP/1.0 200 OK\n");
		strcpy(Buffer3, "");
		count = 0;
		while (!in.eof()) {
			in.read((char *) buf, sizeof buf);
			if (in.gcount()) {
				strncat(Buffer3, (char *) buf, in.gcount());
				count += in.gcount();
			}
		}
		strcat(Buffer2, "Content-Length: ");
		sprintf(buf2, "%d", count);
		strcat(Buffer2, buf2);
		strcat(Buffer2, "\n");

		strcpy(path2, file_name);
		char *q = strrchr(path2, '.');
		if (!strcmp(q, ".html") || !strcmp(q, ".htm"))
			strcat(Buffer2, "Content-Type: text/html\n\n");
		if (!strcmp(q, ".txt"))
			strcat(Buffer2, "Content-Type: text/plain\n\n");
		if (!strcmp(q, ".jpeg") || !strcmp(q, ".jpg"))
			strcat(Buffer2, "Content-Type: image/jpeg\n\n");

		strncat(Buffer2, Buffer3, count);
		in.close();
	}
}

int is_method(char method[])
{
	if (!strcmp(method, "GET") || !strcmp(method, "POST") || !strcmp(method, "HEAD"))
		return 0;
	else
		return -1;
}
