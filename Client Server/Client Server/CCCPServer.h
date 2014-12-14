#pragma once

#include "CCCP.h"

class CCCPServer : public CCCP
{
private:
	bool cmdStart(std::vector<std::string>& parameters);
	bool cmdStop(std::vector<std::string>& parameters);
	bool cmdRequest(std::vector<std::string>& parameters);
	bool cmdLogin(std::vector<std::string>& parameters);

protected:
	void work();
	void listen();
	void setup();
	void prompt();
	bool command(std::vector<std::string>& parameters, bool local = false);
public:
	~CCCPServer();

	void start();
	void stop();
	bool command(std::string command, bool local = false);

	void compile();
};

