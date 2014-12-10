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


#define MAX_NAME_LENGTH 255

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

	char dirName[MAX_NAME_LENGTH];
	int dirNameLen = GetCurrentDirectory(MAX_NAME_LENGTH, dirName);
	

	std::string sliced = getRelativeDir(dirNameLen, dirName);

	//Write the header
	bytes << "ZT";
	bytes << (char)m;
	bytes.write(reinterpret_cast<const char *>(&numFiles), sizeof(numFiles));
	bytes.write(dirName, dirNameLen); // Fix this

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
			//Files
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				names.push(fd.cFileName);
				sizes.push(fd.nFileSizeLow);
				std::cout << fd.cFileName << std::endl;
			}
			//SubDirectories, must recurse
			else
			{
				std::tuple<std::queue<std::string>, std::queue<uLong>> tmpDir = listFiles(fd.cFileName);
				std::queue<std::string>tmpNames = std::get<0>(tmpDir);
				std::queue<uLong>tmpSizes = std::get<1>(tmpDir);
				for (int i = 0; i < tmpNames.size(); i++)
				{
					names.push(tmpNames.front());
					sizes.push(tmpSizes.front());
					tmpNames.pop();
					tmpSizes.pop();
				}
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return std::make_tuple(names, sizes);
}

std::string Preprocessor::getRelativeDir(int length, char* path)
{
	{
		//This block finds the location of the last \ in the path.
		int index = 0;
		for (int i = 0; i < length; i++)
		{
			if (path[i] == '\\')
			{
				index = i;
			}
		}
		//This block copies everything after it into a new string.
		std::string localDirectory;
		for (int i = index + 1; i < length; i++)
		{
			localDirectory += path[i];
		}
		return localDirectory;
	}
}