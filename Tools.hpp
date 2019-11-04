#pragma once

#include <string>
#include <filesystem>

#include "Settings.hpp"

#ifndef LINUX
// NTFS
// 255 �������� - ���� ������ �������� �����
#define MAX_LENGTH_FILENAME 255
#else
// FAT32
// 255 ���� - ���� ������ �������� �����
#define MAX_LENGTH_FILENAME 255
#endif

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