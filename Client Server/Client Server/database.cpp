#include "database.h"
#include <stdlib.h>
#include <stdio.h>
#include <exception>
#include <memory>
#include <time.h>
#include "constants.h"
#include "md5.h"


database::database()
{
	db = nullptr;
}


database::~database()
{
	sqlite3_close_v2(db);
	free(db);
}

int getIDCallback(void* idStore, int colCount, char** values, char** colNames)
{
	*(static_cast<int*>(idStore))= atoi(values[0]);
	return 0;
}

std::string database::generatePassword(std::string username, std::string password)
{
	return md5(username + password);
}

bool database::openDB()
{
	if (db != nullptr)
		return true;

	//Check for db file.
	FILE * dbFile;
	fopen_s(&dbFile, DBFILE, "r");

	//Open db file regardless
	int err = sqlite3_open_v2(DBFILE, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if (err != SQLITE_OK)
		return false;

	//If it didn't exist, execute creation.
	if (dbFile == nullptr)
	{
		if (sqlite3_exec(db, CCCP_CREATE.c_str(), nullptr, 0, nullptr) != SQLITE_OK)
		{
			closeDB();
			return false;
		}
	}
	else
		fclose(dbFile);	

	return true;
}

bool database::closeDB()
{
	if (sqlite3_close_v2(db) != SQLITE_OK)
		return false;
	db = nullptr;
	return true;
}

bool database::addUser(std::string username, std::string password)
{
	std::string hashpass = generatePassword(username, password);
	lasterr = openDB();

	//Will fail if username is taken
	std::string sql = "INSERT INTO auth (username, passwd) VALUES ('" + username + "', X'" + hashpass + "');";
	lasterr = sqlite3_exec(db, sql.c_str(), nullptr, 0, nullptr);
	closeDB();

	if (lasterr != SQLITE_OK)
		return false;

	return true;
}

bool database::removeUser(std::string username)
{
	lasterr = openDB();

	std::string sql = "DELETE FROM auth WHERE username = '" + username + "';";
	lasterr = sqlite3_exec(db, sql.c_str(), nullptr, 0, nullptr);
	closeDB();

	if (lasterr != SQLITE_OK)
		return false;

	return true;
}

bool database::removeUser(int id)
{
	openDB();

	std::string sql = "DELETE FROM auth WHERE id = " + std::to_string(id) + ";";
	lasterr = sqlite3_exec(db, sql.c_str(), nullptr, 0, nullptr);
	closeDB();

	if (lasterr != SQLITE_OK)
		return false;

	return true;
}

bool database::addCompiler(std::string name)
{
	openDB();

	std::string sql = "INSERT INTO system (compiler) VALUES ('"+name+"');";
	lasterr = sqlite3_exec(db, sql.c_str(), nullptr, 0, nullptr);
	closeDB();

	if (lasterr != SQLITE_OK)
		return false;

	return true;
}

bool database::removeCompiler(std::string name)
{
	openDB();

	std::string sql = "DELETE FROM system WHERE compiler = '" + name + "';";
	lasterr = sqlite3_exec(db, sql.c_str(), nullptr, 0, nullptr);
	closeDB();

	if (lasterr != SQLITE_OK)
		return false;

	return true;
}

bool database::removeCompiler(int id)
{
	openDB();

	std::string sql = "DELETE FROM system WHERE id = " + std::to_string(id) + ";";
	lasterr = sqlite3_exec(db, sql.c_str(), nullptr, 0, nullptr);
	closeDB();

	if (lasterr != SQLITE_OK)
		return false;

	return true;
}

bool database::addFlag(std::string compiler, std::string flag, char parameter)
{
	//Get compiler id by name
	std::unique_ptr<int> id = std::unique_ptr<int>(new int(-1));
	openDB();

	std::string sql = "SELECT id FROM system WHERE compiler = '"+compiler+"';";
	lasterr = sqlite3_exec(db, sql.c_str(), getIDCallback, id.get(), nullptr);
	closeDB();

	if (lasterr != SQLITE_OK)
		return false;

	//Forward to overloaded add method
	if (*id > -1)
		addFlag(*id, flag, parameter);
	else
		return false;

	return true;
}

bool database::addFlag(int compiler, std::string flag, char parameter)
{
	openDB();

	std::string sql = "INSERT INTO flags (compiler, flag, parameter) VALUES (" + std::to_string(compiler) + ", '" + flag + "', " + std::to_string(parameter) + ");";
	lasterr = sqlite3_exec(db, sql.c_str(), nullptr, 0, nullptr);
	closeDB();

	if (lasterr != SQLITE_OK)
		return false;

	return true;
}

bool database::removeFlag(std::string compiler, std::string flag)
{
	//Get compiler id by name
	std::unique_ptr<int> id = std::unique_ptr<int>(new int(-1));
	openDB();

	std::string sql = "SELECT id FROM system WHERE compiler = '" + compiler + "';";
	lasterr = sqlite3_exec(db, sql.c_str(), getIDCallback, id.get(), nullptr);
	closeDB();

	if (lasterr != SQLITE_OK)
		return false;

	//Forward to overloaded remove method
	if (*id > -1)
		removeFlag(*id, flag);
	else
		return false;

	return true;
}

bool database::removeFlag(int compiler, std::string flag)
{
	openDB();

	std::string sql = "DELETE FROM flags WHERE compiler = " + std::to_string(compiler) + " AND flag = '"+flag+"';";
	lasterr = sqlite3_exec(db, sql.c_str(), nullptr, 0, nullptr);
	closeDB();

	if (lasterr != SQLITE_OK)
		return false;

	return true;
}

bool database::addSetting(std::string shortname, std::string query, std::string user)
{
	//Get user id by name
	std::unique_ptr<int> id = std::unique_ptr<int>(new int(-1));
	openDB();

	std::string sql = "SELECT id FROM auth WHERE username = '" + user + "';";
	lasterr = sqlite3_exec(db, sql.c_str(), getIDCallback, id.get(), nullptr);
	closeDB();

	if (lasterr != SQLITE_OK)
		return false;

	//Forward to overloaded add method
	if (*id > -1)
		addSetting(shortname, query, *id);
	else
		return false;

	return true;
}

bool database::addSetting(std::string shortname, std::string query, int user)
{
	openDB();

	std::string sql = "INSERT INTO settings (shortname, query, user) VALUES ('" + shortname + "', '" + query + "', " + std::to_string(user) + ");";
	lasterr = sqlite3_exec(db, sql.c_str(), nullptr, 0, nullptr);
	closeDB();

	if (lasterr != SQLITE_OK)
		return false;

	return true;
}

bool database::removeSetting(std::string shortname, std::string user)
{
	//Get user id by name
	std::unique_ptr<int> id = std::unique_ptr<int>(new int(-1));
	openDB();

	std::string sql = "SELECT id FROM auth WHERE username = '" + user + "';";
	lasterr = sqlite3_exec(db, sql.c_str(), getIDCallback, id.get(), nullptr);
	closeDB();

	if (lasterr != SQLITE_OK)
		return false;

	//Forward to overloaded remove method
	if (*id > -1)
		removeSetting(shortname, *id);
	else
		return false;

	return true;
}

bool database::removeSetting(std::string shortname, int user)
{
	openDB();

	std::string sql = "DELETE FROM settings WHERE shortname = '" + shortname + "' AND user = " + std::to_string(user)+"; ";
	lasterr = sqlite3_exec(db, sql.c_str(), nullptr, 0, nullptr);
	closeDB();

	if (lasterr != SQLITE_OK)
		return false;

	return true;
}
/*
bool verifySys(std::string compiler)
{

}

std::tuple<int, int> verifyFlag(std::string flag)
{

}*/

std::string database::login(std::string username, std::string password)
{
	std::unique_ptr<int> id = std::unique_ptr<int>(new int(-1));
	std::string hashpass = generatePassword(username, password);

	openDB();

	//Check for existence
	std::string sql = "SELECT id FROM auth WHERE username = '" + username + "' AND passwd = X'" + hashpass + "';";
	lasterr = sqlite3_exec(db, sql.c_str(), getIDCallback, id.get(), nullptr);
	

	if (*id == -1)
	{
		closeDB();
		return "";
	}

	//Generate a session key
	srand(time(NULL));
	std::string cert = md5(std::to_string(rand()));

	sql = "INSERT INTO session (user, cert) VALUES (" + std::to_string(*id) + ", X'" + cert + "');";
	lasterr = sqlite3_exec(db, sql.c_str(), getIDCallback, id.get(), nullptr);
	closeDB();

	return cert;
}
