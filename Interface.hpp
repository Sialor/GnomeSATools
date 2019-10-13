#pragma once

#define IMG 0
#define TXD 1

#define PACK 0
#define UNPACK 1

#include <string>
#include <iostream>


// ==============================
//
// Класс необходим для обработки запросов.
//
// Следует изменять строки std::clog
// в соответствии с определениями методов
//
// В Windows Aaa.txt и aaa.txt и aAA.txt - одинаковые имена
// ================================


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