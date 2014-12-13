#pragma once

#include "CCCP.h"

class CCCPServer : public CCCP
{
public:
	~CCCPServer();

	void start();
	void stop();
	void command(std::string command);

	void compile();
};

