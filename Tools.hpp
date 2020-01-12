#pragma once

#include <string>
#include <filesystem>
#include <cstring>

#include "Settings.hpp"



struct FileStructureToSearch
{
	char m_name[24];
	unsigned short m_size;
};



// ==============================
//
// ������� ��� ������ � �������
//
// ������� �������� ������ std::clog
// � ������������ � ������������� �������
// ================================


// ���������� Img ������
// ��������� ��� ����� ������� ��������������
// � ���� ����� ������
void ImgUnpack(std::string, std::string);



// ��������� Img ������
// ��������� ���� ����� ������� �������������
// � ���� ����� ������
void ImgPack(std::string, std::string);