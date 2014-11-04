#include "preprocessor.h"

Preprocessor::Preprocessor(std::string path)
{

}


void Preprocessor::compressFile(std::string path)
{

	std::ifstream infile(path, std::ios::binary);
	gzFile outfile = gzopen("compress", "wb");

	char buffer[128];
	int num_read = 0;
	unsigned long total_read = 0, total_wrote = 0;
	while (infile.read(buffer, 128).rdstate() == std::ios_base::goodbit) 
	{
		total_read += num_read;
		gzwrite(outfile, buffer, infile.gcount());
		deflate()
	}

	infile.close();
	gzclose(outfile);
}

void Preprocessor::decompressFile(std::string path)
{
	gzFile infile = gzopen("littleOutput.txt", "rb");
	std::ofstream outfile("enbiggenOutput.txt", std::ios::binary);

	char buffer[128];
	int num_read = 0;
	while ((num_read = gzread(infile, buffer, sizeof(buffer))) > 0) {
		outfile.write(buffer, num_read);
	}

	gzclose(infile);
	outfile.close();
}


std::queue<std::string> Preprocessor::listFiles(std::string path)
{
	std::queue<std::string> names;
	char search_path[200];
	sprintf(search_path, "%s*.*", path.c_str());
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path, &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				names.push(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}