#include "TCPSocket.hpp"

using namespace std;
tcp_error_t TCPSocket::sendBytes(vector<byte> data)
{

}

tcp_error_t TCPSocket::recieveBytes()
{

}

tcp_error_t TCPSocket::sendString(std::string msg)
{
	int res = send(connectSocket, msg.c_str(), msg.length(), 0);
	if (res == SOCKET_ERROR)
	{
		std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
		closesocket(connectSocket);
		WSACleanup();
		return;
	}
	std::cout << "Bytes sent: " << res << std::endl;
	return SUCCESS;
}

tcp_error_t TCPSocket::recieveString()
{


}