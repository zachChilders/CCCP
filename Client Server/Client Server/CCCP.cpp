#include "CCCP.h"
#include "CCCPClient.h"
#include "CCCPServer.h"


CCCP::CCCP()
{ 
	started = false; 
}

CCCP* CCCP::Create(CCCP::State type)
{
	if (type == State::client)
		return new CCCPClient();
	else
		return new CCCPServer();
}

void CCCP::command(std::string command)
{

}

void send(std::string message)
{

}

std::string receive()
{
	return "";
}