#include "CCCPClient.h"
#include "TCPClient.h"
#include <iostream>
#include <modes.h>
#include <osrng.h>
#include <vector>
#include <iterator>
#include <conio.h>
#include <thread>

using std::cout;
using std::cerr;
using std::cin;
using std::string;
using std::vector;
using std::thread;
using namespace CryptoPP;


CCCPClient::~CCCPClient()
{
	stop();
}

void CCCPClient::promptPass()
{
	password = "";

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
}

void CCCPClient::start()
{
	if (started)
		return;

	//Get server and login information.
	string ip, user, pass;
	cout << "Enter server IP: ";
	cin >> ip;	

	//Reset our unique pointer
	connection.reset(new TCPClient(ip.c_str()));
	username = user;
	password = pass;

	/*First handshake: Key exchange*/
	AutoSeededRandomPool rnd;

	aesKey = SecByteBlock(0x00, AES::DEFAULT_KEYLENGTH);
	rnd.GenerateBlock(aesKey, aesKey.size());
	rnd.GenerateBlock(aesIV, AES::BLOCKSIZE);

	//Message is our key then iv contiguously.
	vector<byte> message;
	message.reserve(AES::BLOCKSIZE * 2);
	message.insert(message.end(), aesKey.begin(), aesKey.end());
	message.insert(message.end(), std::begin(aesIV), std::end(aesIV));

	cout << "Connecting to server...\n";
	connection->start();

	connected = true;

	cout << "Encrypting connection...\n";
	send(message, false);

	//Clear vector and get the response.
	string response = receiveString();

	if (response != "OkieDokie")
	{
		cerr << "Error encrypting connection.\n";
		stop();
		return;
	}

	//It's officially started.
	started = true;

	//Prompt for login info.
	command(receiveString());

	if (sessionKey.empty())
		return;
	work();
}

void CCCPClient::stop()
{
	if (!started)
		return;

	connection->stop();
	username.erase();
	password.erase();
	sessionKey.erase();
	started = false;
	connected = false;
	listening = false;
	encrypted = false;
}

void CCCPClient::compile()
{

}

void CCCPClient::login()
{
	cout << "Enter username: ";
	cin >> username;
	cout << "Enter password: ";
	promptPass();
	
	send("login " + username + ' ' + password);
	command(receiveString());
}

void CCCPClient::work()
{
	thread listener(&CCCPClient::listen, this);
	prompt();
}

void CCCPClient::prompt()
{
	string input;
	cin.ignore();

	while (promptable)
	{
		cout << "CCCP@" << connection->getIP() << "> ";
		std::getline(cin, input);
		command(input, true);
	}
}

//Constantly tries to receive. Should be run on another thread.
void CCCPClient::listen()
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
		if (listening && !recv.empty())
			command(recv);
	}
	listening = false;
}