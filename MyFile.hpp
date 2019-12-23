#pragma once

#include <iostream>
#include <string>
#include <Windows.h>

#include "Settings.hpp"


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
	unsigned long long m_size;

	// ������� ���� ����, � ������ ����� ���������� ������/������
	unsigned long long m_index;

	char* m_data;

	// ���� ������������
	// ����� ����� ������ � �������
	static bool m_isNotStream;



	// ���������� �������� �� ��,
	// ���������� �� ������������ �����
	// ������ ������.
	// ���� ����� ������, ��������� ��������
	// ������������� ������ ������.
	// ��������� ������ ������� ����������� �� ��������
	void isIndexOverflow(unsigned long long index)
	{
		// index == m_size - ������ ��� ������/������ ���������� � "������"
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



	// ������ �� ������ n ����
	// ������ �� ����������, ���������
	// ��������� ������������ � ������ ���������
	void readUCharP(char *, unsigned long long);



	// ������ �� ������ ���� ����
	// ���������� unsigned char
	unsigned char readUChar();



	// ������ �� ������ ��� �����
	// ���������� unsigned short
	unsigned short readUShort();



	// ������ �� ������ ������ �����
	// ���������� unsigned int
	unsigned int readUInt();



	// ������ � ������ n ����
	void writeUCharP(char *, unsigned long long);



	// ������ � ������ ���� ����
	void writeUChar(unsigned char);



	// ������ � ������ ��� �����
	void writeUShort(unsigned short);



	// ������ � ������ ������ �����
	void writeUInt(unsigned int);



	// �������� � ������ n ���� � ������� m_index
	// ��������� ������ ������������� �����
	// ������ ���������� ������� �� �������
	// 2 * file_size + insert_data_size
	// �� ����� �������������� ������� (�������� � �����)
	// ��������� �� ����� ����������������
	void insertBytes(unsigned long long);



	// ������� �� ������ n ���� � ������� m_index
	// ��������� ��� ������������� ��� � �������������
	// ������������� ������� �� ������
	void removeBytes(long long);


	
	// ������� ������� �� ��������� �������
	void goToByte(unsigned long long);
	


	// ���������� n-�� ���������� ����
	// ����� ��������� ������������� �������� ��� ����������� �����
	void skipBytes(long long);



	// ��������� �������
	unsigned long long getIndex();



	// ��������� ������� �����
	unsigned long long& getSize();



	// ��������� ������� ����� ���������� � ��������
	static unsigned long long getSizeFile(std::string);



	// ���������� ��������� �� ������ � ������ � �������
	// �� ���������� �����
	char* getPointerData(unsigned long long index=0);



	// ��������� ���������� ���� � m_data
	// ��������� ���������� ����, ������ �������������
	void createData(unsigned long long);



	// ���������� ���� isNotStream
	static bool getIsNotStream();



	// ������������� ���� isNotStream � ��������� ��������
	static void setIsNotStream(bool);
};