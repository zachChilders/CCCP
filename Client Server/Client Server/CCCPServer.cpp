#include "CCCPServer.h"
#include "TCPServer.hpp"

CCCPServer::~CCCPServer()
{
	stop();
}

void CCCPServer::start()
{
	if (started)
		return;

	connection = std::unique_ptr<TCPServer>(new TCPServer());
	connection->start();
}

void CCCPServer::stop()
{
	connection->stop();
}

void CCCPServer::command(std::string command)
{

}

void CCCPServer::compile()
{

}
