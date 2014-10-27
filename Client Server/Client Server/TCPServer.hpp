#include "TCPSocket.hpp"

#include <iostream>
#include <thread>
#include <vector>

class TCPServer : TCPSocket
{
	public:
	TCPServer();
	~TCPServer();

	private:
	std::vector<char> recvbuf;
	int res;

	bool running;

	struct addrinfo *result, *ptr, hints;
	SOCKET listenSocket;
	SOCKET clientSocket;

	void open();
	void bindSocket();
	void listenOnSocket();
	void work();
};
