#include "TCPServer.hpp"
#include "TCPClient.h"
#include "preprocessor.h"
#include <fstream>
#include <stdlib.h>
#include <zlib.h>

int main()
{

	//Server Test 
	/*TCPServer s;+
	s.start();
	*/
	//Client Test
	//TCPClient c("172.22.1.82");
	//c.start();
	//c.stop();

	//s.stop();

	Preprocessor p;
	std::tuple<std::queue<std::string>, std::queue<unsigned long>> tup = p.listFiles("./");
	p.compressDir(tup, (MODE)0);
}