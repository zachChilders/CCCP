#include "TCPServer.hpp"
#include "TCPClient.h"

int main() 
{
	//Server Test 
	TCPServer s;
	s.start();

	////Client Test
	//TCPClient c("172.22.1.82");
	//c.start();
	//c.stop();

	s.stop();
	
}