#include "Interface.h"

int main(int argc, char **argv)
{
	const char *_argv[] = {".exe", "img", "pack", "player.img", "gta3.img" };

	Interface interface(5, (char**)_argv);

	return 0;
}