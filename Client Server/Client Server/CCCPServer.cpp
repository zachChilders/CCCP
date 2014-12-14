#include "CCCPServer.h"
#include "TCPServer.hpp"
#include "database.h"
#include <iostream>
#include <string>
#include <thread>

using std::cout;
using std::cin;
using std::cerr;
using std::vector;
using std::thread;
using std::string;
using namespace CryptoPP;

CCCPServer::~CCCPServer()
{
	stop();
}

void CCCPServer::start()
{
	if (started)
		return;

	threads["server"].reset(new thread(&CCCPServer::setup, this));
	prompt();
}

void CCCPServer::setup()
{
reset:
	if (started)
		return;

	started = true;
	//Reset our unique pointer
	connection.reset(new TCPServer());
	connection->start();

	//In case we stopped in the middle of trying to connect.
	if (!started)
		return;

	//Get the key and IV from client
	vector<byte> data;
	data = receive(false);
	aesKey.Assign((byte*)&data[0], AES::BLOCKSIZE);
	for (int i = 0; i < AES::BLOCKSIZE; i++)
		aesIV[i] = (byte)data[i + AES::BLOCKSIZE];

	//Acknowledge that we got the key.
	send("OkieDokie");
	connected = true;

	//Check for login. If fail, reset the server.
	send("request login");
	command(receiveString());
	if (sessionKey.empty())
	{
		stop();
		goto reset;
	}
	

	cout << "\nCCCP" << (connection->getIP().empty() ? "" : "@" + connection->getIP()) << ">";
	threads["listening"].reset(new thread(&CCCPServer::listen, this));
}

void CCCPServer::stop()
{
	if (!started)
		return;

	listening = false;
	connection->stop();
	started = false;
	connected = false;
	encrypted = false;
	database db;
	//db.
}

void CCCPServer::compile()
{

}

void CCCPServer::prompt()
{
	if (prompting)
		return;

	prompting = true;
	string input, ipString = "";
	cin.ignore();

	while (promptable)
	{
		if (connection.get() != nullptr)
			ipString = (connection->getIP().empty() ? "" : "@" + connection->getIP());
		cout << "CCCP" << ipString << ">";
		std::getline(cin, input);
		command(input, true);
	}
	prompting = false;
}

//Constantly tries to receive. Should be run on another thread.
void CCCPServer::listen()
{
	if (listening)
		return;

	string recv;
	listening = true;

	//Non-blocking mode for the socket. This way it's easy to stop listening.
	u_long mode = 1;
	ioctlsocket(connection->getSocket(), FIONBIO, &mode);

	while (listening)
	{
		recv = receiveString(encrypted);

		//In case we stop listening while receiving a message.
		if(listening && !recv.empty())
			command(recv);
	}
	listening = false;
}