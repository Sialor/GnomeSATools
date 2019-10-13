#pragma once

#define IMG 0
#define TXD 1

#define PACK 0
#define UNPACK 1

#include <string>
#include <iostream>


// ==============================
//
// ����� ��������� ��� ��������� ��������.
//
// ������� �������� ������ std::clog
// � ������������ � ������������� �������
//
// � Windows Aaa.txt � aaa.txt � aAA.txt - ���������� �����
// ================================


class Interface
{
private:
	char m_tool;

	char m_command;

	char m_countFiles;

	std::string *m_fileNames;
public:
	Interface(int, char**);

	~Interface();
};