#include "MyFile.hpp"

#include <fstream>

MyFile::MyFile() : m_size(0), m_index(0), m_data(nullptr)
	/*—начала списки инициализации, потом выполн€етс€ конструктор*/
{
#ifdef DEBUG
	std::clog << "MyFile::MyFile()\n";
#endif
}

MyFile::MyFile(std::string fileName) : MyFile()
{
	this->copyDataFromFile(fileName);

#ifdef DEBUG
	std::clog << "MyFile::MyFile(" << fileName << ")\n";
#endif
}

MyFile::~MyFile()
{
	if (m_data != nullptr)
	{
		delete[] m_data;
	}

#ifdef DEBUG
	std::clog << "MyFile::~MyFile()\n";
#endif
}



void MyFile::copyDataFromFile(std::string fileName)
{
	std::ifstream file(fileName, std::ios::binary);

	if (!file.is_open())
	{
#ifdef DEBUG
		std::cerr << "Exception \"The file for some reason does not open\" "
			"in void MyFile::copyDataFromFile(" << fileName << ")\n";
#endif
		throw "The file for some reason does not open";
	}

	file.seekg(0, std::ios::end);
	
	m_size = file.tellg();

	if (m_data != nullptr)
	{
		delete[] m_data;
	}

	m_data = new char[m_size];

	file.seekg(0, std::ios::beg);
	
	file.read(m_data, m_size);

	file.close();

	m_index = 0;

#ifdef DEBUG
	std::clog << "void MyFile::copyDataFromFile(" << fileName << ")\n";
#endif
}



void MyFile::copyDataToFile(std::string fileName)
{
	if (m_data == nullptr)
	{
#ifdef DEBUG
		std::cerr << "Exception \"Before writing a file, you must open this file in RAM\" "
			"in void MyFile::copyDataToFile(" << fileName << ")\n";
#endif
		throw "The file for some reason does not open";
	}

	std::ofstream file(fileName, std::ios::binary);

	if (!file.is_open())
	{
#ifdef DEBUG
		std::cerr << "Exception \"The file for some reason does not open\" "
			"in void MyFile::copyDataToFile(" << fileName << ")\n";
#endif
		throw "The file for some reason does not open";
	}

	file.write(m_data, m_size);

	file.close();

#ifdef DEBUG
	std::clog << "void MyFile::copyDataToFile(" << fileName << ")\n";
#endif
}



void MyFile::readUCharP(char *cstr, unsigned long long count)
{
	isIndexOverflow(m_index + count);

	for (unsigned long long i = 0; i < count; ++i)
	{
		cstr[i] = m_data[m_index + i];
	}

	m_index += count;

#ifdef DEBUG
	std::clog << "void MyFile::readUCharP( [DATA] , " << (int)count << ")\n";
#endif
}



unsigned char MyFile::readUChar()
{
	isIndexOverflow(m_index);

#ifdef DEBUG
	std::clog << "unsigned char MyFile::readUChar()\n";
#endif

	return m_data[m_index++];
}



unsigned short MyFile::readUShort()
{
	isIndexOverflow(m_index + 2);

	unsigned short value(0);

	value = ((unsigned char)(m_data[m_index + 1]) << 8) + 
			(unsigned char)(m_data[m_index + 0]);

	m_index += 2;

#ifdef DEBUG
	std::clog << "unsigned short MyFile::readUShort()\n";
#endif

	return value;
}



unsigned int MyFile::readUInt()
{
	isIndexOverflow(m_index + 4);

	unsigned int value(0);

	value = ((unsigned char)(m_data[m_index + 3]) << 24) +
			((unsigned char)(m_data[m_index + 2]) << 16) +
			((unsigned char)(m_data[m_index + 1]) << 8) +
			(unsigned char)(m_data[m_index + 0]);

	m_index += 4;

#ifdef DEBUG
	std::clog << "unsigned int MyFile::readUInt()\n";
#endif

	return value;
}



void MyFile::writeUCharP(char *cstr, unsigned long long count)
{
	isIndexOverflow(m_index + count);

	for (unsigned int i = 0; i < count; ++i)
	{
		m_data[m_index + i] = cstr[i];
	}

	m_index += count;

#ifdef DEBUG
	std::clog << "void MyFile::writeUCharP( [DATA] , " << (int)count << ")\n";
#endif
}



void MyFile::writeUChar(unsigned char value)
{
	isIndexOverflow(m_index);

	m_data[m_index] = value;

	++m_index;

#ifdef DEBUG
	std::clog << "void MyFile::writeUChar(" << value << ")\n";
#endif
}



void MyFile::writeUShort(unsigned short value)
{
	isIndexOverflow(m_index + 2);

	m_data[m_index + 0] = (unsigned char)(value >> 0);
	m_data[m_index + 1] = (unsigned char)(value >> 8);

	m_index += 2;

#ifdef DEBUG
	std::clog << "void MyFile::writeUShort(" << value << ")\n";
#endif
}



void MyFile::writeUInt(unsigned int value)
{
	isIndexOverflow(m_index + 4);

	m_data[m_index + 0] = (unsigned char)(value >> 0);
	m_data[m_index + 1] = (unsigned char)(value >> 8);
	m_data[m_index + 2] = (unsigned char)(value >> 16);
	m_data[m_index + 3] = (unsigned char)(value >> 24);

	m_index += 4;

#ifdef DEBUG
	std::clog << "void MyFile::writeUInt(" << value << ")\n";
#endif
}



void MyFile::insertBytes(unsigned long long count)
{
	if (count == 0)
	{
		return;
	}

	char* data = new char[m_size + count];

	for (unsigned long long i(0); i < m_index; ++i)
	{
		data[i] = m_data[i];
	}

	for (unsigned long long i(m_index); i < m_size; ++i)
	{
		data[i + count] = m_data[i];
	}

	m_size += count;

	if (m_data != nullptr)
	{
		delete[] m_data;
	}

	m_data = data;

	// ѕослед добавлени€ байт, сместить каретку
	// ¬место этой строчки можно использовать skip
	//m_index += count;

#ifdef DEBUG
	std::clog << "void MyFile::insertBytes(" << count << ")\n";
#endif
}



void MyFile::removeBytes(long long count)
{
	if (count == 0)
	{
		return;
	}

	isIndexOverflow(m_index + count);

	char* data = new char[m_index - abs(count) == 0 ? 1 : m_index - abs(count)];

	for (unsigned long long i(0); i < m_index - abs(count); ++i)
	{
		data[i] = m_data[i];
	}

	for (unsigned long long i(m_index); i < m_size; ++i)
	{
		data[i - abs(count)] = m_data[i];
	}

	m_size -= abs(count);

	if (count < 0)
	{
		m_index -= abs(count);
	}

	if (m_data != nullptr)
	{
		delete[] m_data;
	}

	m_data = data;

#ifdef DEBUG
	std::clog << "void MyFile::removeBytes(" << count << ")\n";
#endif
}



void MyFile::goToByte(unsigned long long position)
{
	isIndexOverflow(position);

	m_index = position;

#ifdef DEBUG
	std::clog << "void MyFile::goToByte(" << position << ")\n";
#endif
}



void MyFile::skipBytes(long long size)
{
	isIndexOverflow(m_index + size);

	m_index += size;

#ifdef DEBUG
	std::clog << "void MyFile::skipBytes(" << size << ")\n";
#endif
}



unsigned long long MyFile::getIndex()
{
#ifdef DEBUG
	std::clog << "unsigned int MyFile::getPosition()\n";
#endif

	return m_index;
}



unsigned long long MyFile::getSize()
{
#ifdef DEBUG
	std::clog << "long long MyFile::getSize()\n";
#endif

	return m_size;
}



unsigned long long MyFile::getSizeFile(std::string fileName)
{
	std::ifstream file;

	file.open(fileName);

	if (!file.is_open())
	{
#ifdef DEBUG
		std::cerr << "Exception \"Could not open file\" "
			"in unsigned long long getSizeFile(" << fileName << ") in class \"MyFile\"\n";
#endif
		throw "Could not open file";
	}

	file.seekg(0, std::ios::end);

	return file.tellg();
}



void MyFile::createData(unsigned long long count)
{
	if (count == 0)
	{
#ifdef DEBUG
		std::cerr << "Exception \"Cannot create an array of size 0\" "
			"in void MyFile::createData(" << count << ")\n";
#endif
		throw "Cannot create an array of size 0";
	}

	if (m_data != nullptr)
	{
		delete[] m_data;
	}

	m_data = new char[count];
	m_size = count;
	m_index = 0;

#ifdef DEBUG
	std::clog << "void MyFile::createData(" << count << ")\n";
#endif
}