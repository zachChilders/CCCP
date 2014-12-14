#include "CCCPServer.h"
#include "database.h"
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::cin;
using std::cerr;
using std::string;
using std::thread;
using std::vector;

bool CCCPServer::command(std::string input, bool local)
{
	if (input.empty())
		return false;

	vector<string> parameters;

	{
		string split = "";
		for (int i = 0; i < input.size(); i++)
		{
			if (input[i] == ' ' || input[i] == '\t')
			{
				parameters.push_back(split);
				split = "";
			}
			else
				split += input[i];
		}
		if (!split.empty())
			parameters.push_back(split);
	}
	parameters[0] = toLower(parameters[0]);
	
	if (!CCCP::command(parameters, local))
		if (!command(parameters, local))
			return false;

	return true;
}

bool CCCPServer::command(vector<string>& parameters, bool local)
{
	//Start some service
	if (parameters[0] == "start" && local)
	{
		parameters.erase(parameters.begin());
		cmdStart(parameters);
	}
	//Stop some service
	else if (parameters[0] == "stop" && local)
	{
		parameters.erase(parameters.begin());
		cmdStop(parameters);
	}
	else if (parameters[0] == "adduser" && local)
	{
		parameters.erase(parameters.begin());
		cmdAddUser(parameters);
	}
	else if (parameters[0] == "removeuser")
	{
		parameters.erase(parameters.begin());
		cmdRemoveUser(parameters);
	}
	else if (parameters[0] == "addcompiler")
	{
		parameters.erase(parameters.begin());
		cmdAddCompiler(parameters);
	}
	else if (parameters[0] == "removecompiler")
	{
		parameters.erase(parameters.begin());
		cmdRemoveCompiler(parameters);
	}
	else if (parameters[0] == "addflag")
	{
		parameters.erase(parameters.begin());
		cmdAddFlag(parameters);
	}
	else if (parameters[0] == "removeflag")
	{
		parameters.erase(parameters.begin());
		cmdRemoveFlag(parameters);
	}
	else if (parameters[0] == "addSetting")
	{
		parameters.erase(parameters.begin());
		cmdAddSetting(parameters);
	}
	else if (parameters[0] == "removeSetting")
	{
		parameters.erase(parameters.begin());
		cmdRemoveSetting(parameters);
	}
	else
		return false;

	return true;
}

//Start commands for Server
bool CCCPServer::cmdStart(std::vector<std::string>& parameters)
{
	if (parameters.size() < 1)
	{
		cerr << "Usage: start [server|listening]\n";
		return true;
	}

	string parameter = toLower(parameters.front());
	if (parameter == "server")
	{
		if (started)
			cout << "Server already started.\n";
		else
		{
			threads["server"].reset(new thread(&CCCPServer::setup, this));
			cout << "Started listening for a connection.\n";
		}
	}
	else if (parameter == "listening")
	{
		if (!connected)
			cerr << "A connection must be present to listen.\n";
		else if (listening)
			cerr << "Server already listening.\n";
		else
		{
			threads["listening"].reset(new thread(&CCCPServer::listen, this));
			cout << "Started listening for messages.\n";
		}
	}
	else
	{
		cerr << "Usage: start [server|listening]\n";
		return false;
	}

	parameters.erase(parameters.begin());

	if (parameters.size() > 0)
		command(parameters);

	return true;
}

//Stop commands for Server
bool CCCPServer::cmdStop(std::vector<std::string>& parameters)
{
	if (parameters.size() < 1)
	{
		cerr << "Usage: stop [server|listening]\n";
		return true;
	}

	string parameter = toLower(parameters.front());
	if (parameter == "server")
	{
		if (!started)
			cout << "Server not running.\n";
		else
		{
			stop();
			threads["server"]->join();
			threads.erase("server");
			cout << "Stopped listening for a connection.\n";
		}
	}
	else if (parameter == "listening")
	{
		if (!connected || !listening)
			cerr << "Server already not listening.\n";
		else
		{
			listening = false;
			threads["listening"]->join();
			threads.erase("listening");
			cout << "No longer listening for messages.\n";
		}
	}
	else
	{
		cerr << "Usage: stop [server|listening]\n";
		return false;
	}

	parameters.erase(parameters.begin());

	if (parameters.size() > 0)
		command(parameters);

	return true;
}

bool CCCPServer::cmdLogin(std::vector<std::string>& parameters)
{
	if (parameters.size() < 2)
	{
		sessionKey = "";
		return true;
	}

	database db;
	sessionKey = db.login(parameters[0], parameters[1]);

	if (sessionKey.empty())
		send("failure No such username or password.");
	else
		send("sessionid " + sessionKey);
}

bool CCCPServer::cmdAddUser(std::vector<std::string>& parameters)
{
	database db;

	if (parameters.size() == 2)
	{
		db.addUser(parameters[0], parameters[1]);
		return true;
	}

	string username, password;

	cout << "Username: ";
	cin >> username;
	cout << "Password: ";
	password = promptPass();

	db.addUser(username, password);
	cout << "User " + username + " added.\n";
	cin.ignore();
}

bool CCCPServer::cmdRemoveUser(std::vector<std::string>& parameters)
{
	database db;
	bool status = false;

	if (parameters.size() == 1)
	{
		if (!db.removeUser(parameters[0]))
			cerr << "No user " + parameters[0];

		return true;
	}

	string username;

	cout << "Username: ";
	cin >> username;

	db.addUser(username, password);
	cout << "User " + username + " added.\n";
	cin.ignore();
}

bool CCCPServer::cmdAddCompiler(std::vector<std::string>& parameters)
{
	database db;

	if (parameters.size() == 2)
	{
		db.addUser(parameters[0], parameters[1]);
		return true;
	}

	string username, password;

	cout << "Username: ";
	cin >> username;
	cout << "Password: ";
	password = promptPass();

	db.addUser(username, password);
	cout << "User " + username + " added.\n";
	cin.ignore();
}