#include "CCCPServer.h"
#include "TCPServer.hpp"

using std::cout;
using std::vector;
using namespace CryptoPP;

CCCPServer::~CCCPServer()
{
	stop();
}

void CCCPServer::start()
{
	if (started)
		return;

	//Reset our unique pointer
	connection.reset(new TCPServer());
	connection->start();

	//Get the key and IV from client
	vector<byte> data;
	connection->receiveBytes(data);
	aesKey.Assign((byte*)&data[0], AES::BLOCKSIZE);
	for (int i = 0; i < AES::BLOCKSIZE; i++)
		aesIV[i] = (byte)data[i + AES::BLOCKSIZE];

	//Acknowledge that we got the key.
	connection->sendBytes(encrypt("OkieDokie"));

	started = true;
}

void CCCPServer::stop()
{
	connection->stop();
}

void CCCPServer::command(std::string command)
{

}

void CCCPServer::compile()
{

}
