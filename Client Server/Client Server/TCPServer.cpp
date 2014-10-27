#include "TCPServer.hpp"

TCPServer::TCPServer()
{
	running = true;
	listenSocket = INVALID_SOCKET;
	clientSocket = INVALID_SOCKET;

	//Init winsock and hints
	res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0)
	{
		std::cout << "WSAStartup Failed: " << res << std::endl;
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;


	//Resolve server address and port
	res = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (res != 0)
	{
		std::cout << "getaddrinfo failed: " << res << std::endl;
		WSACleanup();
		return;
	}

}

TCPServer::~TCPServer()
{
	res = shutdown(clientSocket, SD_SEND);
	closeSocket(clientSocket);
	WSACleanup();
}

void TCPServer::open()
{
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET)
	{
		std::cout << "Error at socket: " << WSAGetLastError() << std::endl;
		freeaddrinfo(res);
		WSACleanup();
		return;
	}
}

void TCPServer::bind()
{
	res = bind(listenSocket, res->ai_addr, res->addrelen)
	{
		if (res == SOCKET_ERROR)
		{
			std::cout << "Bind failed with error: " << WSAGetLastError() << std::endl;
			freeaddrinfo(res);
			closesocket(listenSocket);
			WSACleanup();
			return;
		}
	}

	freeaddrinfo(res);
}

void TCPServer::listen()
{
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "Listen failed with error: " << WSAGetLastError() << std::endl;
		freeaddrinfo(res);
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	clientSocket = accept(listenSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "Accept failed; " << WSAGetLastError() << std::endl;
		closesocket(listeSocket);
		WSACleanup();
		return;
	}
}

void TCPServer::work()
{
	//Work loop - recieves data and echoes it back.
	do {
		res = recv(clientSocket, recvbuf, recvbuflen, 0);
		if (res > 0){
			std::cout << "Bytes recieved: " << res << "\n";

			//Echo back to the sender
			iSendResult = send(clientSocket, recvbuf, res, 0);
			if (iSendResult == SOCKET_ERROR){
				std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
				closesocket(clientSocket);
				WSACleanup();
				return;
			}
			std::cout << "Bytes sent: " << iSendResult << std::endl;

		}
		else if (res == 0){
			std::cout << "Connection closing...." << std::endl;
		}
		else{
			std::cout << "recv failed: " << WSAGetLastError() << std::endl;
			closesocket(clientSocket);
			WSACleanup();
			return;
		}
	} while (res > 0);
}

tcp_error_t TCPServer::start()
{
	open();
	bind();
	while (running)
	{
		listen();
		std::thread t(work);
	}
}