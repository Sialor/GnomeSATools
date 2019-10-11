#pragma once

#define DEBUG

#define IMG 0
#define TXD 1

#define PACK 0
#define UNPACK 1

#include <string>
#include <iostream>

class Interface
{
private:
	char m_tool;

	char m_command;

	char m_countFiles;

	std::string *m_fileNames;
public:
	Interface(int, char**);

	~Interface();
};