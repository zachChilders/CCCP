#include "CCCP.h"
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::cin;
using std::cerr;
using std::string;
using std::vector;

string CCCP::toLower(string in)
{
	std::transform(in.begin(), in.end(), in.begin(), ::tolower);
	return in;
}

bool CCCP::command(std::vector<string> parameters, bool local)
{
	//Send a message to connection
	if (parameters[0] == "send" && local)
	{
		parameters.erase(parameters.begin());
		return cmdSend(parameters);
	}
	else if (parameters[0] == "message")
	{
		parameters.erase(parameters.begin());
		return cmdMessage(parameters);
	}
	else if (parameters[0] == "failure")
	{
		parameters.erase(parameters.begin());
		return cmdFailure(parameters);
	}
	else
		return false;
}

bool CCCP::cmdSend(std::vector<std::string>& parameters)
{
	if (!connected)
	{
		cerr << "Connection needed to send a message.\n";
		return true;
	}

	string message;
	if (parameters.empty())
	{
		cout << '>';
		std::getline(cin, message);
	}
	else
		for (int i = 0; i < parameters.size(); i++)
			message += parameters[i];

	send("message " + message);
	return true;
}

bool CCCP::cmdMessage(std::vector<std::string>& parameters)
{
	if (parameters.empty())
		return true;

	string message = "";
	for (int i = 0; i < parameters.size(); i++)
		message += parameters[i];

	cout << "\n" + connection->getIP() + " says: " + message + '\n';

	string ipString;
	if (connection.get() != nullptr)
		ipString = (connection->getIP().empty() ? "" : "@" + connection->getIP());
	cout << "CCCP" << ipString << ">";

	return true;
}

bool CCCP::cmdFailure(std::vector<std::string>& parameters)
{
	if (parameters.empty())
		return true;

	string message = "";
	for (int i = 0; i < parameters.size(); i++)
		message += (i > 0 ? " " : "")+parameters[i];

	cerr << "\n" + connection->getIP() + " says: " + message + '\n';

	return true;
}