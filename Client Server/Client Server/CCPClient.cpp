#include "CCPClient.h"
#include <iostream>
//#include "include\Encryption\modes.h"
//#include <osrng.h>

using std::cout;
using std::cin;
using std::string;
using namespace CryptoPP;

CCCPClient::CCCPClient()
{
}

void CCCPClient::start()
{
	//Get server and login information.
	string ip, user, pass;
	cout << "Enter server IP: ";
	cin >> ip;
	cout << "Enter username: ";
	cin >> user;
	cout << "Enter password: ";
	cin >> pass;

	connection = std::unique_ptr<TCPClient>(new TCPClient(ip.c_str()));
	username = user;
	password = pass;

	//First handshake: Key exchange
	//AutoSeededRandomPool rnd;
	//aesKey = SecByteBlock(0x00, AES::DEFAULT_KEYLENGTH);
	//CryptoPP::CFB_Mode<AES>	
}

void CCCPClient::stop()
{

}

void CCCPClient::command(std::string command)
{

}

void CCCPClient::compile()
{

}