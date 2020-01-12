#pragma once

#include <string>
#include <filesystem>
#include <cstring>

#include "Settings.hpp"



struct FileStructureToSearch
{
	char m_name[24];
	unsigned short m_size;
};



// ==============================
//
// Функции для работы с файлами
//
// Следует изменять строки std::clog
// в соответствии с определениями методов
// ================================


// Распаковка Img архива
// Принимает имя файла которое распаковываешь
// и путь папки вывода
void ImgUnpack(std::string, std::string);



// Запаковка Img архива
// Принимает путь папки которую запаковываешь
// и путь папки вывода
void ImgPack(std::string, std::string);