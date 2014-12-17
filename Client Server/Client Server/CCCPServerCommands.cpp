#include "CCCPServer.h"
#include "database.h"
#include "CMDRunner.h"
#include <iostream>
#include <fstream>
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
	else if (parameters[0] == "removeuser" && local)
	{
		parameters.erase(parameters.begin());
		cmdRemoveUser(parameters);
	}
	else if (parameters[0] == "addcompiler" && local)
	{
		parameters.erase(parameters.begin());
		cmdAddCompiler(parameters);
	}
	else if (parameters[0] == "removecompiler" && local)
	{
		parameters.erase(parameters.begin());
		cmdRemoveCompiler(parameters);
	}
	else if (parameters[0] == "addflag" && local)
	{
		parameters.erase(parameters.begin());
		cmdAddFlag(parameters);
	}
	else if (parameters[0] == "removeflag" && local)
	{
		parameters.erase(parameters.begin());
		cmdRemoveFlag(parameters);
	}
	else if (parameters[0] == "addsetting")
	{
		parameters.erase(parameters.begin());
		cmdAddSetting(parameters, local);
	}
	else if (parameters[0] == "removesetting")
	{
		parameters.erase(parameters.begin());
		cmdRemoveSetting(parameters, local);
	}
	else if (parameters[0] == "login")
	{
		parameters.erase(parameters.begin());
		cmdLogin(parameters);
	}
	else if (parameters[0] == "compile")
	{
		parameters.erase(parameters.begin());
		cmdCompile(parameters);
	}
	else if (parameters[0] == "compiledemo")
	{
		parameters.erase(parameters.begin());
		cmdCompileDemo(parameters);
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

	return true;
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

	return true;
}

bool CCCPServer::cmdRemoveUser(std::vector<std::string>& parameters)
{
	database db;

	if (parameters.size() == 1)
	{
		if (!db.removeUser(parameters[0]))
			cerr << "No user " + parameters[0] + '\n';

		return true;
	}

	string username;

	cout << "Username: ";
	cin >> username;

	if(db.removeUser(username))
		cout << "User " + username + " removed.\n";
	else
		cerr << "No user " + parameters[0] + '\n';

	return true;
}

bool CCCPServer::cmdAddCompiler(std::vector<std::string>& parameters)
{
	database db;

	if (parameters.size() == 1)
	{
		db.addCompiler(parameters[0]);
		return true;
	}

	string compiler;

	cout << "Compiler name: ";
	cin >> compiler;

	db.addCompiler(compiler);
	cout << "Compiler " + compiler + " added.\n";
	cin.ignore();
	return true;
}

bool CCCPServer::cmdRemoveCompiler(std::vector<std::string>& parameters)
{
	database db;

	if (parameters.size() == 1)
	{
		if (!db.removeUser(parameters[0]))
			cerr << "No user " + parameters[0] + '\n';

		return true;
	}

	string compiler;

	cout << "Compiler name: ";
	cin >> compiler;

	if (db.removeUser(compiler))
		cout << "Compiler " + compiler + " removed.\n";
	else
		cerr << "No compiler " + parameters[0] + ".\n";

	cin.ignore();
	return true;
}

bool CCCPServer::cmdAddFlag(std::vector<std::string>& parameters)
{
	database db;

	if (parameters.size() == 2)
	{
		db.addFlag(parameters[0], parameters[1], false);
		return true;
	}
	else if (parameters.size() == 3)
	{
		db.addFlag(parameters[0], parameters[1], true);
		return true;
	}

	string compiler, flag;
	int parameter;

	cout << "Compiler name: ";
	cin >> compiler;

	if (!db.verifySys(compiler))
	{
		char a;
		cout << "Compiler does not exist. Would you like to add it? (Y/n)";
		cin >> a;

		if (a == 'Y' || a == 'y')
			db.addCompiler(compiler);
		else
		{
			cerr << "Adding flag cancelled.\n";
			return true;
		}
	}

	cout << "Flag (no - or /): ";
	cin >> flag;

	cout << "Number of parameters: ";
	cin >> parameter;

	if (db.addFlag(compiler, flag, parameter))
		cout << "Flag " + flag + " for compiler " + compiler + " added.\n";
	else
		cerr << "Flag " + flag + " not added.\n";
	cin.ignore();
	return true;
}

bool CCCPServer::cmdRemoveFlag(std::vector<std::string>& parameters)
{
	database db;

	if (parameters.size() == 2)
	{
		if (!db.removeFlag(parameters[0], parameters[1]))
			cerr << "No user " + parameters[0] + '\n';

		return true;
	}

	string compiler, flag;

	cout << "Compiler name: ";
	cin >> compiler;

	cout << "Flag name: ";
	cin >> flag;

	if (db.removeFlag(compiler, flag))
		cout << "Flag " + flag + " for compiler " + compiler + " removed.\n";
	else
		cerr << "No flag " + flag + " for compiler " + compiler + ".\n";
	cin.ignore();
	return true;
}

bool CCCPServer::cmdAddSetting(vector<string>& parameters, bool local)
{
	string shortname, query = "", user;
	if (parameters.size() < 2 && !local)
	{
		cerr << "failure Usage: addsetting shortname query";
		return true;
	}

	database db;
	
	if (!local)
	{
		for (int i = 1; i < parameters.size(); i++)
			query += (i > 1 ? " " : "") + parameters[i];

		CMDRunner cmd;
		cmd.setDB(&db);
		if (cmd.verifyCmd(query))
		{
			int user = db.getUser(sessionKey);
			if (db.addSetting(parameters[0], query, user))
				cout << "Setting " + shortname + " saved.\n";
			else
				cerr << "Could not add " + shortname + "\n";
			return true;
		}
		else
		{
			cerr << "failure Query could not be verified.";
			return true;
		}
	}

	if (local && parameters.size() < 1)
	{
		cout << "Shortname: ";
		cin >> shortname;
	}
	else
		shortname = parameters[0];
	
	if (local && parameters.size() < 2)
	{
		cout << "User: ";
		cin >> user;
	}
	else
		user = parameters[1];

	if (local && parameters.size() < 3)
	{
		cout << "Query: ";
		cin.ignore();
		std::getline(cin, query);
	}
	else
		for (int i = 2; i < parameters.size(); i++)
			query += (i > 0 ? " " : "") + parameters[i];

	if (db.addSetting(shortname, query, user))
		cout << "Setting " + shortname + " added.\n";
	else
		cout << "Could not add setting.\n";

	return true;
}

bool CCCPServer::cmdRemoveSetting(std::vector<std::string>& parameters, bool local)
{
	string shortname, query = "", user;
	if (parameters.size() < 1 && !local)
	{
		cerr << "failure Usage: removesetting shortname\n";
		return true;
	}

	database db;

	if (!local)
	{
		for (int i = 0; i < parameters.size(); i++)
			query += parameters[i];

		int user = db.getUser(sessionKey);
		if (db.removeSetting(shortname, user))
			cout << "Setting " + shortname + " removed.\n";
		else
			cerr << "No settings removed.\n";

		return true;
	}

	if (local && parameters.size() < 1)
	{
		cout << "Shortname: ";
		cin >> shortname;
	}

	if (local && parameters.size() < 2)
	{
		cout << "User: ";
		cin >> user;
	}

	if (db.removeSetting(shortname, user))
		cout << "Setting " + shortname + " removed.\n";
	else
		cerr << "No settings removed.\n";
	cin.ignore();
	return true;
}

bool CCCPServer::cmdCompile(std::vector<std::string>& parameters)
{
	CMDRunner cmdr;

	u_long mode = 0;
	ioctlsocket(connection->getSocket(), FIONBIO, &mode);

	//Receive 3 commands
	string fname = receiveString();
	
	//Hopefully string won't break the source
	string file = receiveString();
	std::ofstream outFile(fname);
	outFile.write(file.c_str(), file.size());
	outFile.close();

	string command = receiveString();

	mode = 1;
	ioctlsocket(connection->getSocket(), FIONBIO, &mode);

	//Reconcat all the parameters into one function
	for (int i = 0; i < parameters.size(); i++)
	{
		command += parameters[i];
	}

	if (!cmdr.verifyCmd(command))
	{
		return false;
	}
	cmdr.run(command);
	return true;
}

bool CCCPServer::cmdCompileDemo(std::vector<std::string>& parameters)
{
	//Set Blocking
	u_long mode = 0;
	ioctlsocket(connection->getSocket(), FIONBIO, &mode);

	//Receive 3 commands
	string fname = receiveString();
	string outName = receiveString();

	//Hopefully string won't break the source
	string file = receiveString();
	string command = receiveString();

	//Remove Blocking
	mode = 1;
	ioctlsocket(connection->getSocket(), FIONBIO, &mode);

	std::ofstream outFile(fname);
	outFile.write(file.c_str(), file.size());
	outFile.close();

	CMDRunner cmdr;
	if (!cmdr.verifyCmd(command))
	{
		return false;
	}
	cmdr.run(command);

	system(outName.c_str());

	std::ifstream inFile("output.txt");
	string test;
	std::getline(inFile, test);
	inFile.close();
	send("message " + test);

	return true;
}