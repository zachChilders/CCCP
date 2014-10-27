#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define DEFAULT_PORT "45067"
#define DEFAULT_BUFLEN 512

#include "tcp_error_t.h"

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <IPHlpApi.h>
#include <string>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")

class TCPSocket
{
	protected:
		std::string address;
		std::string port;
		WSADATA wsaData;
		SOCKET connectSocket;
	
	public:
		virtual tcp_error_t start() = 0;
		virtual tcp_error_t stop() = 0;

		virtual tcp_error_t sendFile(std::string path);
		virtual tcp_error_t recieveFile();

		virtual tcp_error_t sendString(std::string msg);
		virtual tcp_error_t recieveString();
		
};