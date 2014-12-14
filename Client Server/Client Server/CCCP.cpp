#include "CCCP.h"
#include "CCCPClient.h"
#include "CCCPServer.h"
#include <iostream>
#include <modes.h>
#include <string>
#include <conio.h>

using std::cin;
using std::cout;
using std::cerr;
using std::string;
using std::vector;
using namespace CryptoPP;

CCCP::CCCP()
{ 
	started = false; 
	connected = false;
	listening = false;
	prompting = false;
	promptable = true;
	encrypted = true;
}

CCCP* CCCP::Create(CCCP::State type)
{
	if (type == State::client)
		return new CCCPClient();
	else
		return new CCCPServer();
}

void CCCP::send(std::string message, bool encrypted)
{
	if (message.empty())
		return;

	if (aesKey.BytePtr() != nullptr && encrypted)
		connection->sendBytes(encrypt(message));
	else
		connection->sendString(message);
}

void CCCP::send(vector<byte> data, bool encrypted)
{
	if (data.empty())
		return;

	if (aesKey.BytePtr() != nullptr && encrypted)
		connection->sendBytes(encrypt(data));
	else
		connection->sendBytes(data);
}

string CCCP::receiveString(bool encrypted)
{
	vector<byte> message = receive(encrypted);

	if (message.size() > 0)
		return string((char*)&message[0], message.size());
	else
		return "";
}

vector<byte> CCCP::receive(bool encrypted)
{
	vector<byte> message;
	connection->receiveBytes(message);

	if (!message.empty() && aesKey.BytePtr() != nullptr && encrypted)
		message = decrypt(message);

	return message;
}

std::vector<byte>& CCCP::encrypt(std::vector<byte>& data)
{
	vector<byte>* message = new std::vector<byte>();
	message->resize(data.size(), 0);

	CFB_Mode<AES>::Encryption cfbEncrypt;
	cfbEncrypt.SetKeyWithIV(aesKey, aesKey.size(), aesIV);
	cfbEncrypt.ProcessData(&(*message)[0], &(data[0]), data.size());

	return *message;
}

std::vector<byte>& CCCP::encrypt(std::string data)
{
	return encrypt(std::vector<byte>(data.begin(), data.end()));
}

std::vector<byte>& CCCP::decrypt(std::vector<byte>& message)
{
	vector<byte>* data = new std::vector<byte>();
	data->resize(message.size(), 0);

	CFB_Mode<AES>::Decryption cfbDecrypt;
	cfbDecrypt.SetKeyWithIV(aesKey, aesKey.size(), aesIV);
	cfbDecrypt.ProcessData(&(*data)[0], &(message[0]), message.size());

	return *data;
}

string CCCP::promptPass()
{
	string password = "";

	//Prompt user for password, but keep it hidden.
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
	
	return password;
}