#pragma once
#include "CCCP.h"


class CCCPClient : public CCCP
{
private:
	std::string username;
	std::string password;
	
	void login();
	void work();
	void listen();
	void prompt();

	bool command(std::vector<std::string>& parameters, bool local = false);

	bool cmdRequest(std::vector<std::string>& parameters);
	bool cmdSessionID(std::vector<std::string>& parameters);
	bool cmdAddSetting(std::vector<std::string>& parameters);
	bool cmdRemoveSetting(std::vector<std::string>& parameters);
	bool cmdCompile(std::vector<std::string>& parameters);
	bool cmdCompileDemo(std::vector<std::string>& parameters);

public:
	~CCCPClient();

	void start();
	void stop();
	bool command(std::string command, bool local = false);

	void compile();

	void setUsername(std::string user);
	std::string getUsername();

	void setPassword(std::string pass);
};

