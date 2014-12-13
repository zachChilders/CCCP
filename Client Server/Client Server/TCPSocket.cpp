#include "TCPSocket.hpp"

using namespace std;
using std::vector;
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
	//std::cout << "Bytes sent: " << res << std::endl;
	return SUCCESS;
}

tcp_error_t TCPSocket::receiveBytes(vector<byte>& out)
{
	int res = recv(connectSocket, (char*)recvbuf.get(), DEFAULT_BUFLEN, 0);
	if (res > 0)
	{
		cout << endl;
		for (int i = 0; i < res; i++)
			out.push_back(recvbuf.get()[i]);
		cout << endl;
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