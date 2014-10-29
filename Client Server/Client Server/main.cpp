#include "TCPServer.hpp"
#include "TCPClient.h"
#include <fstream>
#include <stdlib.h>
#include <zlib.h>

int main() 
{


	char *buf = new char[128];
	int num_read = 0;

	FILE *inFile = fopen("Output.txt", "rb");
	gzFile out = gzopen("littleOutput.txt", "wb");
	
	while ((num_read = fread(buf, 1, 128, inFile)) > 0)
	{
		gzwrite(out, buf, num_read);
	}
	fclose(inFile);
	gzclose(out);

	FILE *outFile = fopen("BigOutput.txt", "wb");
	gzFile inCompress = gzopen("littleOutput.txt", "rb");
	
	while (num_read = gzread(inCompress, buf, 128) > 0)
	{
		fwrite(buf, 1, num_read, outFile);
	}
	fclose(outFile);
	gzclose(inCompress);

	//Server Test 
	/*TCPServer s;+
	s.start();
*/
	//Client Test
	//TCPClient c("172.22.1.82");
	//c.start();
	//c.stop();

	//s.stop();
	
}