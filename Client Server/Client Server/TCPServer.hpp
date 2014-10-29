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

		tcp_error_t sendBytes(std::vector<byte> data);
		tcp_error_t recieveBytes();

		tcp_error_t sendString(std::string msg);
		tcp_error_t recieveString();
};
