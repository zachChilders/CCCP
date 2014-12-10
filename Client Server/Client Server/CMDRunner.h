#include <Windows.h>
#include <regex>
#include <string>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <tuple>

#include "database.h"

class CMDRunner
{
	private:
		database* db;

	public:
		CMDRunner();
		~CMDRunner();

		void setDB(database* database);
		void verifyCmd(std::string cmd);
};

CMDRunner::CMDRunner()
{
	db = nullptr;
}

CMDRunner::~CMDRunner()
{
	delete db;
}

void CMDRunner::setDB(database* database)
{
	db = database;
}

void CMDRunner::verifyCmd(std::string cmd)
{
	//Split into string array
	std::vector<std::string> cmdTokens;

	std::istringstream iss(cmd);
	std::string tempToken;
	while (std::getline(iss, tempToken, ' '))
	{
		cmdTokens.push_back(tempToken);
	}

	//The first token will be a compiler, verify it
	if (!db->verifySys(cmdTokens[0]))
	{
		std::cerr << "Invalid compiler." << std::endl;
		return;
	}

	//Use regex to find the flags
	std::string pattern = "";
	std::regex r(pattern);
	for (int i = 0; i < cmdTokens.size(); i++)
	{
		//Match found
		if (std::regex_match(cmdTokens[i], r))
		{
			std::tuple<int, int> verify = db->verifyFlag(cmdTokens[i]);
			//Verified
			if (std::get<0>(verify))
			{
				//Verify the next parameters
				int numFlags = std::get<1>(verify);
				for (int j = 0; j < numFlags; j++)
				{
					//If it matches the flag pattern, something is wrong
					if (std::regex_match(cmdTokens[i + j], r))
					{
						std::cerr << "Invalid parameter: " << cmdTokens[i + j] << std::endl;
						return;
					}
				}
				//If we get this far, we should skip ahead to the next flag	
			}
			//Invalid Flags
			else
			{
				std::cerr << "Invalid flag: " << cmdTokens[i] << std::endl;
				return;
			}
		}
	}

	//Run the command through winprocess.
	CreateProcess(NULL, cmd.c_str);

}

