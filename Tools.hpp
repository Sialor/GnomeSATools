#pragma once

#include <string>
#include <filesystem>

#include "Settings.hpp"

#ifndef LINUX
// NTFS
// 255 символов - макс длинна названия файла
#define MAX_LENGTH_FILENAME 255
#else
// FAT32
// 255 байт - макс длинна названия файла
#define MAX_LENGTH_FILENAME 255
#endif

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