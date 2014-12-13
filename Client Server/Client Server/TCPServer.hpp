#pragma once
#include "TCPSocket.hpp"

#include <iostream>
#include <vector>

class TCPServer : public TCPSocket
{
	public:
		TCPServer();
		~TCPServer();
		tcp_error_t start();
		tcp_error_t stop();
		SOCKET clientSocket;

	private:
		int res;
		bool running;

		struct addrinfo *result, *ptr, hints;

		void open();
		void bindSocket();
		void listenOnSocket();
		void work();
};
