#pragma once

#include <string>
#include <fstream>
#include <iostream>

#define DEBUG

// ==============================
//
// ����� ��������� ��� ������������������ ������ � �������.
// ������ ����������� ����� ����, ���������� ��������
// ���� �����, � ������������ � ����������.
//
// ������� �������� ������ std::clog
// � ������������ � ������������� �������
//
// ================================


class MyFile
{
private:
	unsigned int m_size;

	// ������� ���� ����, � ������ ����� ���������� ������/������
	unsigned int m_index;

	char* m_data;



	// ���������� �������� �� ��,
	// ���������� �� ������������ �����
	// ������ ������.
	// ���� ����� ������, ��������� ��������
	// ������������� ������ ������.
	// ��������� ������ ������� ����������� �� ��������
	void isIndexOverflow(unsigned int index)
	{
		// index == m_size - ������ ��� ������/������ ���������� � "������"
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



	// ������ �� ������ �� ������ � n ����
	// ������ �� ����������, ���������
	// ��������� ������������ � ������ ���������
	void readUCharP(unsigned char *, unsigned char);



	// ������ �� ������ ���� ����
	// ���������� unsigned char
	unsigned char readUChar();



	// ������ �� ������ ��� �����
	// ���������� unsigned short
	unsigned short readUShort();



	// ������ �� ������ ������ �����
	// ���������� unsigned int
	unsigned int readUInt();



	// ������ � ������ �� ������ n ����
	void writeUCharP(unsigned char *, unsigned char);



	// ������ � ������ ���� ����
	void writeUChar(unsigned char);



	// ������ � ������ ��� �����
	void writeUShort(unsigned short);



	// ������ � ������ ������ �����
	void writeUInt(unsigned int);



	// �������� � ������ n ���� � ������� m_position
	// ��������� ������ ������������� �����
	// ������ ���������� ������� �� �������
	// 2 * file_size + insert_data_size
	void insertBytes(unsigned int);



	// ������� �� ������ n ���� � ������� m_position
	// ��������� ��� ������������� ��� � �������������
	void removeBytes(int);


	
	// ������� ������� �� ��������� �������
	void goToByte(unsigned int);
	


	// ���������� n-�� ���������� ����
	// ����� ��������� ������������� �������� ��� ����������� �����
	void skipBytes(int);



	// ��������� �������
	unsigned int getIndex();



	// ��������� ������� �����
	unsigned int getSize();
};