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



	// ���������� �������� �� ��,
	// ���������� �� ������������ �����
	// ������ ������.
	// ���� ����� ������, ��������� ��������
	// ������������� ������ ������.
	// ��������� ������ �������� ������
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



	// ������ �� ������ ���� ����
	// ���������� unsigned char
	unsigned char readFileUByte();



	// ������ �� ������ ��� �����
	// ���������� unsigned short
	unsigned short readFileUShort();



	// ������ �� ������ ������ �����
	// ���������� unsigned int
	unsigned int readFileUInt();



	// ���������� n-�� ���������� ����
	// ����� ��������� ������������� �������� ��� ����������� �����
	void skipBytes(int);
};