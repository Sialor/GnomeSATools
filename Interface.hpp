#pragma once

#include <iostream>
#include <string>

#include "Settings.hpp"

#define IMG 0
#define TXD 1

#define PACK 0
#define UNPACK 1


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
	unsigned char m_tool;

	unsigned char m_command;

	unsigned char m_numberOfFiles;

	unsigned char m_numberOfParams;

	// ������� ����
	// ����� ���� ��� ���������� ��� � ����� ������
	std::string *m_inputPaths;

	// �������� ����
	// ����� ���� ������ ����������
	std::string m_outputFolderPath;
public:
	Interface(int, char**);

	~Interface();
};