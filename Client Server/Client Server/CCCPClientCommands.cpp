#include "CCCPClient.h"
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::cin;
using std::cerr;
using std::string;
using std::thread;
using std::vector;

bool CCCPClient::command(std::string input, bool local)
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
	toLower(parameters[0]);
	
	if (!CCCP::command(parameters, local))
		if (!command(parameters, local))
			return false;

	return true;
}

bool CCCPClient::command(vector<string>& parameters, bool local)
{
	//Send a message to connection
	if (parameters[0] == "request")
	{
		parameters.erase(parameters.begin());
		return cmdRequest(parameters);
	}
	else if (parameters[0] == "sessionid")
	{
		parameters.erase(parameters.begin());
		return cmdSessionID(parameters);
	}
	else if (parameters[0] == "addsetting")
	{
		parameters.erase(parameters.begin());
		return cmdAddSetting(parameters);
	}
	else
		return false;

}

bool CCCPClient::cmdRequest(std::vector<std::string>& parameters)
{
	if (parameters.empty())
		return true;

	if (parameters[0] == "login")
	{
		login();
	}
	else
		return false;

	return true;
}

bool CCCPClient::cmdSessionID(std::vector<std::string>& parameters)
{
	if (parameters.empty())
	{
		cerr << "Error retrieving session ID.\n";
		return true;
	}
	
	sessionKey = parameters[0];

	return true;
}

bool CCCPClient::cmdAddSetting(std::vector<std::string>& parameters)
{
	string shortname, query = "", user;

	if (parameters.size() < 1)
	{
		cout << "Shortname: ";
		cin >> shortname;
	}
	else
		shortname = parameters[0];

	if (parameters.size() < 2)
	{
		cout << "Query: ";
		cin.ignore();
		std::getline(cin, query);
	}
	else
		for (int i = 2; i < parameters.size(); i++)
			query += (i > 0 ? " " : "") + parameters[i];

	send("addsetting " + shortname + ' ' + query);

	return true;
}