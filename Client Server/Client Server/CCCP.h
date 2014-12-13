#pragma once
#include <string>
#include <memory>
#include "TCPSocket.hpp"
#include <aes.h>

class CCCP
{
protected:
	std::unique_ptr<TCPSocket> connection;
	CryptoPP::SecByteBlock aesKey;
	byte aesIV[CryptoPP::AES::DEFAULT_KEYLENGTH];
	CCCP();
	void send(std::string message);
	std::string receive();
public:
	enum State{ client, server };

	static CCCP& Create(State type);

	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void command(std::string command);

	virtual void compile() = 0;
};

