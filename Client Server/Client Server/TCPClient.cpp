#include "TCPClient.h"

TCPClient::TCPClient(char* address)
{
	this->address = address;
	this->port = DEFAULT_PORT;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		throw WSASTARTFAIL;
	}

}

TCPClient::~TCPClient()
{
	WSACleanup();
}

tcp_error_t TCPClient::start()
{
	int iResult;

	struct addrinfo *result = NULL,
					*ptr = NULL,
					hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(address.c_str(), DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		WSACleanup(); //????????????
		return GETADDRFAIL;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		connectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);

		if (connectSocket == INVALID_SOCKET) {
			WSACleanup(); //????????????
			return SOCKETINVALID;
		}

		// Connect to server.
		iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(connectSocket);
			connectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (connectSocket == INVALID_SOCKET) {
		WSACleanup(); //????????????
		return CONNECTIONFAIL;
	}

	return SUCCESS;
}

tcp_error_t TCPClient::stop()
{
	int iResult;
	char recvbuf[DEFAULT_BUFLEN];

	// shutdown the connection since no more data will be sent
	iResult = shutdown(connectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		closesocket(connectSocket);
		WSACleanup();
		return SHUTDOWNFAIL;
	}

	// Receive until the peer closes the connection
	do {

		iResult = recv(connectSocket, recvbuf, DEFAULT_BUFLEN, 0);

	} while (iResult > 0);

	closesocket(connectSocket);

	return SUCCESS;
}