#include "suggest.h"

int
main(int argc, char **argv)
{
	if (argc < 2) {
		std::cout << "Little arguments" << std::endl;
		return 1;
	}

	int port = atoi(argv[1]);
	int MasterSocket;
	int Result;

	MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MasterSocket == -1) {
		std::cout << strerror(errno) << std::endl;
		return 1;
	}

	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(port);
	SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	struct sockaddr *sa = (struct sockaddr *)&SockAddr;
	Result = bind(MasterSocket, sa, sizeof(SockAddr));
	if (Result  == -1) {
		std::cout << strerror(errno) << std::endl;
		return 1;
	}

	if ((Result = listen(MasterSocket, SOMAXCONN)) == -1) {
		std::cout << strerror(errno) << std::endl;
		return 1;
	}

	std::vector<std::thread> threads;
	std::list<int> list[4];
	std::mutex m[4];

	for (int i = 0; i < 4; i++)
    	threads.push_back(std::thread(worker, &list[i], &m[i]));

	int st = 0;
	while(true)
	{
		int SlaveSocket = accept(MasterSocket, 0, 0);
		if (SlaveSocket == -1) {
			std::cout << strerror(errno) << std::endl;
			return 1;
		}

		m[st].lock();
		list[st].push_back(SlaveSocket);
		m[st].unlock();

		st = (st + 1) % 4;
	}

	return 0;
}
