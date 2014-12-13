#include "CCCP.h"
#include "CCCPClient.h"
#include "CCCPServer.h"
#include <modes.h>

using std::vector;
using namespace CryptoPP;

CCCP::CCCP()
{ 
	started = false; 
}

CCCP* CCCP::Create(CCCP::State type)
{
	if (type == State::client)
		return new CCCPClient();
	else
		return new CCCPServer();
}

void CCCP::command(std::string command)
{

}

void CCCP::send(std::string message)
{

}

std::string CCCP::receive()
{
	return "";
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