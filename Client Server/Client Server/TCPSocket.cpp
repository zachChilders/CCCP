#include "TCPSocket.hpp"

using namespace std;
tcp_error_t TCPSocket::sendBytes(vector<byte> data)
{
	return SUCCESS;
}

tcp_error_t TCPSocket::recieveBytes()
{
	return SUCCESS;
}

tcp_error_t TCPSocket::sendString(std::string msg)
{
	int res = send(connectSocket, msg.c_str(), msg.length(), 0);
	if (res == SOCKET_ERROR)
	{
		std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
		closesocket(connectSocket);
		WSACleanup();
		return CONNECTIONFAIL;
	}
	std::cout << "Bytes sent: " << res << std::endl;
	return SUCCESS;
}

tcp_error_t TCPSocket::recieveString()
{
	int res = recv(connectSocket, (char*) recvbuf.get(), DEFAULT_BUFLEN, 0);
	if (res > 0)
	{
		return SUCCESS;
	}
	else
	{
		return CONNECTIONFAIL;
	}

}