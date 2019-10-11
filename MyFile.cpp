#include "MyFile.hpp"

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



// Public:
MyFile::MyFile() : m_size(0), m_position(0), m_data(nullptr)
	/*Сначала списки инициализации, потом выполняется конструктор*/
{
#ifdef DEBUG
	std::clog << "MyFile::MyFile() in class \"MyFile\"\n";
#endif
}

MyFile::MyFile(std::string fileName) : MyFile()
{
	this->copyDataFromFile(fileName);

#ifdef DEBUG
	std::clog << "MyFile::MyFile(std::string fileName) in class \"MyFile\"\n";
#endif
}

MyFile::~MyFile()
{
	if (m_data != nullptr)
	{
		delete[] m_data;
	}

#ifdef DEBUG
	std::clog << "MyFile::~MyFile() in class \"MyFile\"\n";
#endif
}



void MyFile::copyDataFromFile(std::string fileName)
{
	std::ifstream file(fileName, std::ios::binary);

	if (!file.is_open())
	{
#ifdef DEBUG
		std::cerr << "Exception \"The file for some reason does not open\" "
			"in void MyFile::copyDataFromFile(std::string fileName) in class \"MyFile\"\n";
#endif
		throw "The file for some reason does not open";
	}

	file.seekg(0, std::ios::end);
	
	m_size = file.tellg();

	m_data = new char[m_size];

	file.seekg(0, std::ios::beg);
	
	file.read(m_data, m_size);

	file.close();

#ifdef DEBUG
	std::clog << "void MyFile::copyDataFromFile(std::string fileName) in class \"MyFile\"\n";
#endif
}



unsigned char MyFile::readFileUByte()
{
	isIndexOverflow(1);

#ifdef DEBUG
	std::clog << "unsigned char MyFile::readFileUByte() in class \"MyFile\"\n";
#endif

	return m_data[m_position++];
}



unsigned short MyFile::readFileUShort()
{
	isIndexOverflow(2);

	unsigned short value(0);

	value = (m_data[m_position + 0] << 8) + 
			m_data[m_position + 1];

	m_position += 2;

#ifdef DEBUG
	std::clog << "unsigned short MyFile::readFileUShort() in class \"MyFile\"\n";
#endif

	return value;
}



unsigned int MyFile::readFileUInt()
{
	isIndexOverflow(4);

	unsigned int value(0);

	value = (m_data[m_position + 0] << 24) +
			(m_data[m_position + 0] << 16) +
			(m_data[m_position + 0] << 8) +
			m_data[m_position + 1];

	m_position += 4;

#ifdef DEBUG
	std::clog << "unsigned int MyFile::readFileUInt() in class \"MyFile\"\n";
#endif

	return value;
}



void MyFile::skipBytes(int size)
{
	isIndexOverflow(size);

	m_position += size;

#ifdef DEBUG
	std::clog << "void MyFile::skipBytes(int size) in class \"MyFile\"\n";
#endif
}