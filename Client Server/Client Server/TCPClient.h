#pragma once

#include "TCPSocket.hpp"

#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class TCPClient : public TCPSocket
{
private:

protected:

public:
	//Initializes Winsock, port, and address for connection
	TCPClient(char* address);

	//Cleans up winsock.
	~TCPClient();

	//Creates a socket and attempts to connect with the stored info.
	tcp_error_t start();

	//Properly closes the connection, but does not clean up Winsock
	tcp_error_t stop();
};