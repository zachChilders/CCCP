/*
* Header Bytes (2) ZT
* Mode (1) Enum { COMPRESSED = 1, ENCRYPTED = 2 }
* Entries (4) int
* Root Directory Name (var) delimiter 
* Size of original block (4)
* Size of compressed data (4)
* File Name (var) relative path
* Data (var)
*
*/

#include "preprocessor.h"
#include <fstream>
#include <memory>

Preprocessor::Preprocessor(std::string path)
{

}

void Preprocessor::decompressDir(std::string path)
{
	std::ifstream bytes(path, std::ios::binary);
	char header[3];
	bytes.get(header, 3);

	if (strcmp(header, "ZT") != 0)
	{
		throw new std::exception("Wrong file type");
	}

	char mode;
	bytes.get(mode);

	//Check flags here

	//Get number of entries
	char bEntries[4];
	bytes.read(bEntries, 4);
	int entries = *reinterpret_cast<int*>(bEntries);

	//Get root name
	std::string rootName = "";
	char in;
	while ((in = bytes.get()) != '\0')
	{
		rootName += in;
	}

	for (int i = 0; i < entries; i++)
	{
		//Get data size information
		char bUncompressedSize[sizeof(uLong)], bCompressedSize[sizeof(uLong)];
		uLong uncompressedSize, compressedSize;

		bytes.read(bUncompressedSize, sizeof(uLong));
		bytes.read(bCompressedSize, sizeof(uLong));

		uncompressedSize = *reinterpret_cast<int*>(bUncompressedSize);
		compressedSize = *reinterpret_cast<int*>(bCompressedSize);

		//Get the file name
		std::string fileName = "./Debug/!zipped/";
		while ((in = bytes.get()) != '\0')
		{
			fileName += in;
		}

		//Read and decompress data
		Bytef* uncompressed = new Bytef[uncompressedSize];
		Bytef* compressed = new Bytef[compressedSize];

		bytes.read((char*)compressed, compressedSize);

		int err = uncompress(uncompressed, &uncompressedSize, compressed, compressedSize);

		//Check for errors

		std::ofstream out(fileName, std::ios::binary);
		out.write((char*)uncompressed, uncompressedSize);
		out.close();

		delete[] uncompressed;
		delete[] compressed;
	}

	bytes.close();
}

void Preprocessor::compressDir(std::tuple<std::queue<std::string>, std::queue<uLong>> fileList, MODE m)
{
	//Seperate our tuple
	std::queue<std::string> files = std::get<0>(fileList);
	std::queue<uLong> sizes = std::get<1>(fileList);
	//Create a filestream to zip everything
	std::ofstream bytes("bytes.!zp", std::ios::binary);

	int numFiles = files.size();
	std::string dirname = "CURRENT DIR";
	//Write the header
	bytes << "ZT";
	bytes << (char)m;
	bytes.write(reinterpret_cast<const char *>(&numFiles), sizeof(numFiles));
	bytes.write(dirname.c_str(), dirname.length()+1); // Fix this

	for (int i = 0; i < numFiles; i++)
	{
		unsigned long fileSize = sizes.front();
		//Open file and get its size.
		std::ifstream file(files.front(), std::ios::binary);

		//Get the upper limit size of compressed file and read file into buffer.
		uLong upperLimit = compressBound(fileSize);
		std::unique_ptr<Bytef> compBuffer(new Bytef[upperLimit]), ifileBuffer(new Bytef[fileSize]);

		file.read((char*)ifileBuffer.get(), fileSize);
		file.close();

		//Compress and write the next components
		compress(compBuffer.get(), &upperLimit, ifileBuffer.get(), fileSize);

		bytes.write(reinterpret_cast<const char *>(&fileSize), sizeof(fileSize)); //Original file size
		bytes.write(reinterpret_cast<const char *>(&upperLimit), sizeof(upperLimit)); //Compressed file size
		bytes.write(files.front().c_str(), files.front().length()+1); //File name
		
		for (int j = 0; j < upperLimit; j++)
		{
			bytes << compBuffer.get()[j];
		}

		files.pop();
		sizes.pop();
	}

	bytes.close();

	//send data here
}

std::tuple<std::queue<std::string>, std::queue<uLong>> Preprocessor::listFiles(std::string path)
{
	std::queue<std::string> names;
	std::queue<uLong> sizes;
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