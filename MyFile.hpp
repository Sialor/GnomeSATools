#pragma once

#include <string>
#include <fstream>
#include <iostream>

#define DEBUG

// ==============================
//
// Класс необходим для кроссплатформенной работы с файлами.
// Вместо исправления всего кода, достаточно изменить
// один класс, в соответствии с платформой.
//
// Следует изменять строки std::clog
// в соответствии с определениями методов
//
// ================================


class MyFile
{
private:
	unsigned int m_size;

	// Включая этот байт, с какого байта происходит чтение/запись
	unsigned int m_index;

	char* m_data;



	// Производит проверку на то,
	// происходит ли переполнение после
	// чтения данных.
	// Этот метод закрыт, поскольку работает
	// исключительно внутри класса.
	// Принимает индекс который проверяется на диапазон
	void isIndexOverflow(unsigned int index)
	{
		// index == m_size - значит что чтение/запись происходит в "притык"
		if (index > m_size)
		{
#ifdef DEBUG
			std::cerr << "Exception \"Overflow index\" "
				"in void isIndexOverflow(unsigned char size) in class \"MyFile\"\n";
#endif

			throw "Overflow index (m_position >= m_szie)";
		}
	}
public:
	MyFile();
	
	MyFile(std::string);

	~MyFile();



	void copyDataFromFile(std::string);



	void copyDataToFile(std::string);



	// Чтение из памяти Си строку в n байт
	// Ничего не возвращает, поскольку
	// результат возвращается в первом аргументе
	void readUCharP(unsigned char *, unsigned char);



	// Чтение из памяти один байт
	// Возвращает unsigned char
	unsigned char readUChar();



	// Чтение из памяти два байта
	// Возвращает unsigned short
	unsigned short readUShort();



	// Чтение из памяти четыре байта
	// Возвращает unsigned int
	unsigned int readUInt();



	// Запись в память Си строки n байт
	void writeUCharP(unsigned char *, unsigned char);



	// Запись в память один байт
	void writeUChar(unsigned char);



	// Запись в память два байта
	void writeUShort(unsigned short);



	// Запись в память четыре байта
	void writeUInt(unsigned int);



	// Вставить в память n байт с позиции m_position
	// Принимает только положительные числа
	// Размер занимаемой области по формуле
	// 2 * file_size + insert_data_size
	void insertBytes(unsigned int);



	// Удалить из памяти n байт с позиции m_position
	// Принимает как положительные так и отрицательные
	void removeBytes(int);


	
	// Перевод каретки на указанную позицию
	void goToByte(unsigned int);
	


	// Пропускает n-ое количество байт
	// Может принимать отрицательные значения для перемещения назад
	void skipBytes(int);



	// Получение позиции
	unsigned int getIndex();



	// Получение размера файла
	unsigned int getSize();
};