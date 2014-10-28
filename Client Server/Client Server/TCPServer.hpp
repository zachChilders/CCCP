#pragma once
#include "TCPSocket.hpp"

#include <iostream>
#include <thread>
#include <vector>

class TCPServer : TCPSocket
{
	public:
		TCPServer();
		~TCPServer();
		tcp_error_t start();
		tcp_error_t stop();
		SOCKET clientSocket;

	private:
		char recvbuf[DEFAULT_BUFLEN];
		int res;
		bool running;

		struct addrinfo *result, *ptr, hints;

		void open();
		void bindSocket();
		void listenOnSocket();
		void work();
		void __start__();
};
