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
		bool verifyCmd(std::string cmd);
		void run(std::string cmd);
};

