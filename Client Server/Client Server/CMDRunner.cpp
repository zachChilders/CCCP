#include "CMDRunner.h"


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
	for (int i = 1; i < cmdTokens.size(); i++)
	{
		//Match found
		if (std::regex_match(cmdTokens[i], r))
		{
			int verify = db->verifyFlag(cmdTokens[0], cmdTokens[i]);
			//Verified
			if (verify)
			{
				//Verify the next parameters
				for (int j = 0; j < verify; j++)
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

	STARTUPINFO si;				//You know what pisses me off?
	PROCESS_INFORMATION pi;		//The fact that they didn't name these the same.

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(si));

	CreateProcessA(NULL,				//Name
				  (LPSTR)cmd.c_str(),   			//Command line
				  FALSE,				//Security
				  FALSE,				//Security attributes
				  NULL,					//inherit flags?
				  NORMAL_PRIORITY_CLASS,//process priority
				  NULL,					//environment 
				  NULL,					//current directory
				  &si,					//Startup info
				  &pi					//Process info, we can query this.
				  );

	WaitForSingleObject(pi.hProcess, INFINITY);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

}

