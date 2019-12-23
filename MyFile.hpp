#pragma once

#include <iostream>
#include <string>
#include <Windows.h>

#include "Settings.hpp"


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
	unsigned long long m_size;

	// Включая этот байт, с какого байта происходит чтение/запись
	unsigned long long m_index;

	char* m_data;

	// Флаг определяющий
	// какой метод работы с файлами
	static bool m_isNotStream;



	// Производит проверку на то,
	// происходит ли переполнение после
	// чтения данных.
	// Этот метод закрыт, поскольку работает
	// исключительно внутри класса.
	// Принимает индекс который проверяется на диапазон
	void isIndexOverflow(unsigned long long index)
	{
		// index == m_size - значит что чтение/запись происходит в "притык"
		if (index > m_size)
		{
#ifdef MY_DEBUG
			std::cerr << "Exception \"Overflow index\" "
				"in void isIndexOverflow(" << index << ") in class \"MyFile\"\n";
#endif
			throw "Overflow index (m_index >= m_szie)";
		}
	}
public:
	MyFile();
	
	MyFile(std::string);

	~MyFile();



	void copyDataFromFile(std::string);



	void copyDataToFile(std::string);



	// Чтение из памяти n байт
	// Ничего не возвращает, поскольку
	// результат возвращается в первом аргументе
	void readUCharP(char *, unsigned long long);



	// Чтение из памяти один байт
	// Возвращает unsigned char
	unsigned char readUChar();



	// Чтение из памяти два байта
	// Возвращает unsigned short
	unsigned short readUShort();



	// Чтение из памяти четыре байта
	// Возвращает unsigned int
	unsigned int readUInt();



	// Запись в память n байт
	void writeUCharP(char *, unsigned long long);



	// Запись в память один байт
	void writeUChar(unsigned char);



	// Запись в память два байта
	void writeUShort(unsigned short);



	// Запись в память четыре байта
	void writeUInt(unsigned int);



	// Вставить в память n байт с позиции m_index
	// Принимает только положительные числа
	// Размер занимаемой области по формуле
	// 2 * file_size + insert_data_size
	// Не стоит злоупотреблять методом (например в цикле)
	// поскольку он очень ресурсозатратный
	void insertBytes(unsigned long long);



	// Удалить из памяти n байт с позиции m_index
	// Принимает как положительные так и отрицательные
	// Отрицательные берутся по модулю
	void removeBytes(long long);


	
	// Перевод каретки на указанную позицию
	void goToByte(unsigned long long);
	


	// Пропускает n-ое количество байт
	// Может принимать отрицательные значения для перемещения назад
	void skipBytes(long long);



	// Получение позиции
	unsigned long long getIndex();



	// Получение размера файла
	unsigned long long& getSize();



	// Получение размера файла указанного в аргменте
	static unsigned long long getSizeFile(std::string);



	// Возвращает указатель на данные в памяти с номером
	// НЕ БЕЗОПАСНЫЙ МЕТОД
	char* getPointerData(unsigned long long index=0);



	// Установка количества байт в m_data
	// Принимает количество байт, только положительные
	void createData(unsigned long long);



	// Возвращает флаг isNotStream
	static bool getIsNotStream();



	// Устанавливает флаг isNotStream в указанное значение
	static void setIsNotStream(bool);
};