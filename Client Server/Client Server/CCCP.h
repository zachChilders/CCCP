#pragma once
#include <string>
#include <vector>
#include <memory>
#include "TCPSocket.hpp"
#include <aes.h>
#include <map>
#include <thread>

class CCCP
{
private:
	bool cmdSend(std::vector<std::string>& parameters);
	bool cmdMessage(std::vector<std::string>& parameters);
	bool cmdRequest(std::vector<std::string>& parameters);
	bool cmdFailure(std::vector<std::string>& parameters);
protected:
	bool started;
	bool connected;
	bool listening;
	bool promptable, prompting;
	bool encrypted;

	std::string sessionKey;

	std::map<std::string, std::unique_ptr<std::thread>> threads;

	std::unique_ptr<TCPSocket> connection;

	std::string promptPass();

	CryptoPP::SecByteBlock aesKey;
	byte aesIV[CryptoPP::AES::DEFAULT_KEYLENGTH];
	std::vector<byte>& encrypt(std::vector<byte>& data);
	std::vector<byte>& encrypt(std::string data);
	std::vector<byte>& decrypt(std::vector<byte>& data);

	void send(std::string message, bool encrypted = true);
	void send(std::vector<byte> data, bool encrypted = true);
	std::vector<byte> receive(bool encrypted = true);
	std::string receiveString(bool encrypted = true);
	virtual void listen() = 0;
	virtual void prompt() = 0;

	bool command(std::vector<std::string> command, bool local = false);

	std::string toLower(std::string in);

	CCCP();

public:
	enum State{ client, server };

	static CCCP* Create(State type);

	virtual void start() = 0;
	virtual void stop() = 0;
	
	virtual void compile() = 0;

	bool isStarted() {return started;}
	void complete() {while (started);}
};