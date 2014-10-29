#include <fstream>
#include <string>
#include <zlib.h>

class PreProcessor
{
	public:
		PreProcessor(std::string path);
		PreProcessor();
		~PreProcessor();
	private:
		Byte* compress(std::fstream *file);
		Byte* compress(std::string path);

		Byte* decompress(Byte* file);
		Byte* encrypt(Byte* data);
		Byte* decrypt(Byte* data);
};