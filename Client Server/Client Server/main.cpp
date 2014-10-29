#include "TCPServer.hpp"
#include "TCPClient.h"
#include <fstream>
#include <stdlib.h>
#include <zlib.h>

int main()
{
	{
		std::ifstream infile("Output.txt", std::ios::binary);
		gzFile outfile = gzopen("littleOutput.txt", "wb");
		if (!infile || !outfile) return -1;

		char inbuffer[128];
		int num_read = 0;
		unsigned long total_read = 0, total_wrote = 0;
		while (infile.read(inbuffer, 128).rdstate() == std::ios_base::goodbit) {
			total_read += num_read;
			gzwrite(outfile, inbuffer, infile.gcount());

		}
		infile.close();
		gzclose(outfile);
	}

	{
		gzFile infile = gzopen("littleOutput.txt", "rb");
		std::ofstream outfile("enbiggenOutput.txt", std::ios::binary);
		if (!infile || !outfile) return -1;

		char buffer[128];
		int num_read = 0;
		while ((num_read = gzread(infile, buffer, sizeof(buffer))) > 0) {
			outfile.write(buffer, num_read);
		}

		gzclose(infile);
		outfile.close();
	}

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