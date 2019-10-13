//#include "Interface.hpp"
#include "MyFile.hpp"

int main(int argc, char **argv)
{
	const char *_argv[] = {".exe", "img", "pack", "player.img", "gta3.img" };

	// —ледует проверить работоспособность класса Interface
	//Interface interface(5, (char**)_argv);
	MyFile file;

	file.copyDataFromFile("data.bin");
	file.writeUShort(256);
	file.copyDataToFile("data.bin2");
	file.copyDataFromFile("data.bin2");
	file.goToByte(0);
	std::cout << file.readUShort() << '\n';

	return 0;
}