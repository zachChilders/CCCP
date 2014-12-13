#include "CCCP.h"
#include "CCPClient.h"


CCCP::CCCP(){}

CCCP& CCCP::Create(CCCP::State type)
{
	return *(new CCCPClient());
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