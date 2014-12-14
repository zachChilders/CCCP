#include "TCPSocket.hpp"

using namespace std;
using std::vector;

//Returning with success does not necessarily mean the client received the message.
//It just means the message fit in the buffer.
tcp_error_t TCPSocket::sendBytes(vector<byte> data)
{
	const char* msg = (char*)&data[0];
	int res = send(connectSocket, msg, data.size(), 0);
	if (res == SOCKET_ERROR)
	{
		std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
		closesocket(connectSocket);
		WSACleanup();
		return CONNECTIONFAIL;
	}
	return SUCCESS;
}

tcp_error_t TCPSocket::receiveBytes(vector<byte>& out)
{
	int res = recv(connectSocket, (char*)recvbuf.get(), DEFAULT_BUFLEN, 0);
	if (res > 0)
	{
		for (int i = 0; i < res; i++)
			out.push_back(recvbuf.get()[i]);
		return SUCCESS;
	}
	else
	{
		return CONNECTIONFAIL;
	}
	return SUCCESS;
}

tcp_error_t TCPSocket::sendString(std::string msg)
{
	sendBytes(std::vector<byte>(msg.begin(), msg.end()));
	return SUCCESS;
}

tcp_error_t TCPSocket::receiveString(std::string& out)
{
	vector<byte> bytes;
	receiveBytes(bytes);
	out = std::string((char*)bytes[0], bytes.size());

	return SUCCESS;
}