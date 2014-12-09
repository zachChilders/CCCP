#pragma once

#include <string>
#include "sqlite3.h"

class database
{
private:
	int lasterr;
	sqlite3* db;
	bool openDB();
	bool closeDB();
	std::string generatePassword(std::string username, std::string password);

public:
	database();
	~database();
	
	bool addUser(std::string username, std::string password);
	bool removeUser(std::string username);
	bool removeUser(int id);

	bool addCompiler(std::string name);
	bool removeCompiler(std::string name);
	bool removeCompiler(int id);

	bool addFlag(std::string compiler, std::string flag, int parameter);
	bool addFlag(int compiler, std::string flag, int parameter);
	bool removeFlag(std::string compiler, std::string flag);
	bool removeFlag(int compiler, std::string flag);

	bool addSetting(std::string shortname, std::string query, std::string user);
	bool addSetting(std::string shortname, std::string query, int user);
	bool removeSetting(std::string shortname, std::string user);
	bool removeSetting(std::string shortname, int user);
	
	bool verifySys(std::string compiler);
	int verifyFlag(std::string compiler, std::string flag);

	std::string login(std::string username, std::string password);
};