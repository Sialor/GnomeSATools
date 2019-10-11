#pragma once

#define DEBUG

#include <string>
#include <fstream>
#include <iostream>

class MyFile
{
private:
	unsigned int m_size;

	unsigned int m_position;

	char* m_data;



	// Производит проверку на то,
	// происходит ли переполнение после
	// чтения данных.
	// Этот метод закрыт, поскольку работает
	// исключительно внутри класса.
	// Принимает размер читаемых данных
	void isIndexOverflow(char size)
	{
		if (m_position + size > m_size || m_position + size < 0)
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



	// Чтение из памяти один байт
	// Возвращает unsigned char
	unsigned char readFileUByte();



	// Чтение из памяти два байта
	// Возвращает unsigned short
	unsigned short readFileUShort();



	// Чтение из памяти четыре байта
	// Возвращает unsigned int
	unsigned int readFileUInt();



	// Пропускает n-ое количество байт
	// Может принимать отрицательные значения для перемещения назад
	void skipBytes(int);
};