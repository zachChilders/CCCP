#include "preprocessor.h"

Preprocessor::Preprocessor(std::string path)
{

}


void compress(std::tuple<std::queue<std::string>, std::queue<unsigned long>> fileList, MODE m)
{
	//Seperate our tuple
	std::queue<std::string> files = std::get<0>(fileList);
	std::queue<unsigned long> sizes = std::get<1>(fileList);
	//Create a filestream to zip everything
	std::ofstream bytes("bytes.!zp", std::ios::binary);

	//Write the header
	bytes << "ZT";
	bytes << m;
	bytes << files.size();
	bytes << "CURRENT DIR"; // Fix this

	for (int i = 0; i < files.size(); i++)
	{
		//Compress file
		//Push original size
		//Push compressed size
		//Push compression data
	}

	bytes.close();

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

std::tuple<std::queue<std::string>, std::queue<unsigned long>> Preprocessor::listFiles(std::string path)
{
	std::queue<std::string> names;
	std::queue<unsigned long> sizes;
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
				sizes.push(fd.nFileSizeLow);
				
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return std::make_tuple(names, sizes);
}