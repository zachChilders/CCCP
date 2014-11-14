#pragma once
#include "tcp_error_t.h"

#include <fstream>
#include <string>
#include <vector>
#include <zlib.h>
#include <queue>
#include <tuple>
#include <Windows.h>


typedef enum MODE{RAW=0, COMPRESSED=2, ENCRYPTED=4};
class Preprocessor
{
	private:
		Byte* bytes;
		std::string getRelativeDir(int length, char* path);
	public:
		Preprocessor(std::string path = "");
		~Preprocessor(){};
		Byte* getBytes(MODE m);

		void compressDir(std::tuple<std::queue<std::string>, std::queue<unsigned long>> fileList, MODE m);
		void compressFile(std::string path);
		void decompressDir(std::string path);
		void decompressFile(std::string path);
		Byte* encrypt(Byte* data);
		Byte* decrypt(Byte* data);

		std::tuple<std::queue<std::string>, std::queue<unsigned long>> listFiles(std::string path);
};