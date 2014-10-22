#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define DEFAULT_PORT "1221"
#define DEFAULT_BUFLEN 512


#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <IPHlpApi.h>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

int main() {

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	WSADATA wsaData;
	int iResult, iSendResult;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	//Initialization
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0){
		cout << "WSAStartup Failed: " << iResult << "\n";
		return 1;
	}
	ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

	//Resolve server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0){
		cout << "getaddrinfo Failed: " << iResult << "\n";
		WSACleanup();
		return 1;
	}

	//open a socket
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if(ListenSocket == INVALID_SOCKET){
		cout << "Error at socket(): " << WSAGetLastError() << "\n";
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	//Bind the socket
	iResult = bind(ListenSocket, result->ai_addr, result->ai_addrlen);
	if(iResult == SOCKET_ERROR){
		cout << "Bind failed with error: " << WSAGetLastError() << "\n";
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	
	freeaddrinfo(result);

	//Listen on the socket
	if( listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR){
		cout << "Listen failed with error: " << WSAGetLastError() << "\n";
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	//Accept a client connection
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if ( ClientSocket == INVALID_SOCKET) {
		cout << "accept failed: " << WSAGetLastError() << "\n";
		closesocket(ListenSocket);
		WSACleanup();
		return 1; 
	}

	closesocket(ListenSocket);

	//Work loop - recieves data and echoes it back.
	do {
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if( iResult > 0){
			cout << "Bytes recieved: " << iResult << "\n";

			//Echo back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if(iSendResult == SOCKET_ERROR){
				cout << "send failed with error: " << WSAGetLastError() << "\n";
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			cout << "Bytes sent: " << iSendResult << "\n";

		} else if (iResult == 0){
			cout << "Connection closing....\n";
		}
		else{
			cout << "recv failed: " << WSAGetLastError() << "\n";
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	}while (iResult > 0);

	//Shut down send component
	iResult = shutdown(ClientSocket, SD_SEND);
	if ( iResult == SOCKET_ERROR){
		cout << "shutdown failed: " << WSAGetLastError() << "\n";
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ClientSocket);
	WSACleanup();
	return 0;
}