#include "CCCPClient.h"
#include "TCPClient.h"
#include <iostream>
#include <modes.h>
#include <osrng.h>
#include <vector>
#include <iterator>
#include <conio.h>

using std::cout;
using std::cin;
using std::string;
using std::vector;
using namespace CryptoPP;


CCCPClient::~CCCPClient()
{
	stop();
}

void CCCPClient::promptPass()
{
	password = "";
	char c;
	while ((c = _getch()) != '\r')
	{

		if (c == '\b'){
			if (password.length() > 0){
				cout << "\b \b";
				password.pop_back();
			}
		}
		else
		{
			password += c;
			cout << '*';
		}
	}
	cout << std::endl;
}

void CCCPClient::start()
{
	if (started)
		return;

	//Get server and login information.
	string ip, user, pass;
	cout << "Enter server IP: ";
	cin >> ip;
	cout << "Enter username: ";
	cin >> user;
	cout << "Enter password: ";
	promptPass();	

	connection = std::unique_ptr<TCPClient>(new TCPClient(ip.c_str()));
	username = user;
	password = pass;

	//First handshake: Key exchange
	AutoSeededRandomPool rnd;

	aesKey = SecByteBlock(0x00, AES::DEFAULT_KEYLENGTH);
	rnd.GenerateBlock(aesKey, aesKey.size());
	rnd.GenerateBlock(aesIV, AES::BLOCKSIZE);
	
	vector<byte> message;
	message.reserve(AES::BLOCKSIZE * 2);
	message.insert(message.end(), aesKey.begin(), aesKey.end());
	message.insert(message.end(), std::begin(aesIV), std::end(aesIV));

	CFB_Mode<AES>::Encryption cfbEncrypt;
	cfbEncrypt.SetKeyWithIV(aesKey, aesKey.size(), aesIV);
	cfbEncrypt.ProcessData(&(message[0]), &(message[0]), message.size());

	cout << "Connecting to server...\n";

	connection->start();
	connection->sendBytes(message);
	connection->receiveBytes(message);
}

void CCCPClient::stop()
{
	if (!started)
		return;

	connection->stop();
	username.erase();
	password.erase();
	sessionKey.erase();
}

void CCCPClient::command(std::string command)
{

}

void CCCPClient::compile()
{

}