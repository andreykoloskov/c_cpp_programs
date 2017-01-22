#include "request.h"

void
master(int sock, char *ip, int port)
{
	int MasterSocket;
	int Result;
	static char Buffer[BUFSIZE];

	MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MasterSocket == -1) {
		std::cout << strerror(errno) << std::endl;
		return;
	}

	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(port);
	SockAddr.sin_addr.s_addr = inet_addr(ip);

	struct sockaddr *sa = (struct sockaddr *)&SockAddr;
	Result = bind(MasterSocket, sa, sizeof(SockAddr));
	if (Result  == -1) {
		std::cout << strerror(errno) << std::endl;
		return;
	}

	if ((Result = listen(MasterSocket, SOMAXCONN)) == -1) {
		std::cout << strerror(errno) << std::endl;
		return;
	}

	while(true)
	{
		int SlaveSocket = accept(MasterSocket, 0, 0);
		if (SlaveSocket == -1) {
			std::cout << strerror(errno) << std::endl;
			return;
		} else
			Buffer[0] = '\0';
		sock_fd_write(sock, Buffer, 1, SlaveSocket);
	}
}
