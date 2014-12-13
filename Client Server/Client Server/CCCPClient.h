#pragma once
#include "CCCP.h"


class CCCPClient : public CCCP
{
private:
	std::string serverIP;
	std::string username;
	std::string password;
	std::string sessionKey;
	
	void login();
	void promptPass();

public:
	~CCCPClient();

	void start();
	void stop();
	void command(std::string command);

	void compile();

	void setUsername(std::string user);
	std::string getUsername();

	void setPassword(std::string pass);
};

