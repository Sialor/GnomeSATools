#pragma once

#include <iostream>
#include <string>

#include "Settings.hpp"

#define IMG 0
#define TXD 1

#define PACK 0
#define UNPACK 1


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
	unsigned char m_tool;

	unsigned char m_command;

	unsigned char m_numberOfFiles;

	unsigned char m_numberOfParams;

	// Входные пути
	// Могут быть как директорий так и имена файлов
	std::string *m_inputPaths;

	// Выходной путь
	// Может быть ТОЛЬКО директория
	std::string m_outputFolderPath;
public:
	Interface(int, char**);

	~Interface();
};