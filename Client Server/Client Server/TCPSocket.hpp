#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define DEFAULT_PORT "45067"
#define DEFAULT_BUFLEN 512

#include "tcp_error_t.h"

#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <IPHlpApi.h>
#include <string>
#include <vector>
#include <memory>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")

#pragma once

class TCPSocket
{
	protected:
		std::string address;
		std::string port;
		WSADATA wsaData;
		SOCKET connectSocket;

		std::unique_ptr<byte> recvbuf;
		std::unique_ptr<byte> sendbuff;


	public:
		virtual tcp_error_t start() = 0;
		virtual tcp_error_t stop() = 0;

		tcp_error_t sendBytes(std::vector<byte> data);
		tcp_error_t receiveBytes(std::vector<byte>& out);

		tcp_error_t sendString(std::string msg);
		tcp_error_t receiveString(std::string& out);
		
};