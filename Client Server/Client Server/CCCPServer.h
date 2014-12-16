#pragma once

#include "CCCP.h"

class CCCPServer : public CCCP
{
private:
	bool cmdStart(std::vector<std::string>& parameters);
	bool cmdStop(std::vector<std::string>& parameters);
	bool cmdRequest(std::vector<std::string>& parameters);
	bool cmdLogin(std::vector<std::string>& parameters);
	bool cmdAddUser(std::vector<std::string>& parameters);
	bool cmdRemoveUser(std::vector<std::string>& parameters);
	bool cmdAddCompiler(std::vector<std::string>& parameters);
	bool cmdRemoveCompiler(std::vector<std::string>& parameters);
	bool cmdAddFlag(std::vector<std::string>& parameters);
	bool cmdRemoveFlag(std::vector<std::string>& parameters);
	bool cmdAddSetting(std::vector<std::string>& parameters, bool local);
	bool cmdRemoveSetting(std::vector<std::string>& parameters, bool local);
	bool cmdCompile(std::vector<std::string>& parameters);

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

