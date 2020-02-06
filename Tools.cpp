#include "Tools.hpp"

#include "MyFile.hpp"


/*
==========================TIMING
UNPACK GTA3; Debug; x86; MY_DEBUG = TRUE; STREAM;         3:16
UNPACK GTA3; Debug; x86; MY_DEBUG = FALSE; STREAM;        1:45
UNPACK GTA3; Debug; x86; MY_DEBUG = FALSE; NOT STREAM;    1:32
UNPACK GTA3; Release; x86; MY_DEBUG = FALSE; NOT STREAM;  1:36
UNPACK GTA3; Release; x86; MY_DEBUG = FALSE; STREAM;      1:42

������������ ������ ��������, ��������, � img ������ ����� 127 ��
�� �������� (�������� ������) ������������ ������ ����� ������� img,
�������������� ������� �������, ����� 8191 �� + 127 ��

================TESTING PROGRAM
img pack "D:\Steam\steamapps\common\Grand Theft Auto San Andreas/unpacked/models\player/" -o "D:\Steam\steamapps\common\Grand Theft Auto San Andreas\models"
img unpack "D:\Steam\steamapps\common\Grand Theft Auto San Andreas/models\player.img" -o "D:\Steam\steamapps\common\Grand Theft Auto San Andreas\unpacked/models"

==============IMG STRUCT
4 byte - CHAR[4] ������. ������ VER2
4 byte - DWORD ���������� ������ � ������

�� 0 �� ���������� ������ � ������:
	4 ����� (���������� � ������)
	2 ������ ������ (���������� � ������)
	2 ������ � ������ (���������� � ������) ������ 0
	24 ��� �����

����� ��������������� ���� ����� � �������


==========================DESCRIPTION OF TXD FORMAT
Txd ���� - �� ���� RenderWare binary stream file. ���� ������� �� ������. ������ ������ �������
�� ���������:

====================STRUCT OF SECTION
ID ������(4 �����),
������ ������ - ����� ����� ������ � ������(4 �����),
������������� ������ RenderWare(4 �����)

������������� ������ ���� ���������� �� ���� ���������� ������ � ����� �����.

�������� � GTA ����� ��������� �� 4-������ (������ ��� PS2), 8-������, 16-������, 32-������.

���������� �������� � � �����-������ ������ �����.
����� �����, ����� ��������� ��� �����-����� - ��� �����-����� �����������.
������ ������� �����-������ ���������� ������ ������������, ����� - ������ ���������, � ������� � ������� ��������� ������ ���������� ��������� ������������ ������������� �� ������ ��������.
������� - �������� ������, ��������������, particle-��������, ������� � �.�.

��������� ����������, ����������� ��� ������� � ���� - DXT1 (��� ������� ��� �����-������) � DXT3 (��� ������� � �����-�������).
�� ������������� ��������� ��� ������� particle-��������, ��� ����� ���������� � �����-������ ����� ����������� ������� ��������� ������ ���������.

====================STRUCT OF TXD
Texture Dictionary
    |
	|
	|----> Header
	|		  |
	|		  |----> 2b - ���������� ������� � ������ (� ������� �� 3.6.0.0 (��� �����) - 4 �����)
	|		  |
	|		  |----> 2b - deviceId (������� � ������ 3.6.0.0 (��� �����)) - 1 ��� D3D8-���������, 2 - ��� D3D9.
	|----> Texture Native - ������ ������ � TXD ����� ����������� ��������� ���, � ����������� �� ���������� ������� � ������.
	|		  |
	|		  |----> Struct - ��������� � ������ � ����������� �������� + ���� ��������
	|		  |
	|		  |----> Extension - ���������� ��������� (������ ������������ ���������� � GTA San Andreas � GTA Vice City)
	|----> Extension - ���������� ������ (�� ������������)

	
=================== �������� ����������� ������� ���������� ��������� ��� ���������� ���������
������ ������������ ����������
�����-�����
��������� ���� ������
*/


bool MyFile::m_isNotStream(false);

void ImgUnpack(std::string fileName, std::string outputFolderPath)
{
	// ���� ������� �������������
	MyFile input(fileName);

	// ������ 4 ����, ��� ������ �����
	char signature[5] = "";

	input.readUCharP(signature, 4);

	// �������� ������
	if (std::string(signature) != "VER2")
	{
		std::cerr << "Exception \"Signature mismatch\" "
			"void ImgUnpack(" << fileName << ", " << outputFolderPath << ")\n";

		throw "Signature mismatch";
	}

	// �������� ����� �����
	std::string pathFolderWithFileName;

	pathFolderWithFileName = outputFolderPath + fileName.substr(fileName.find_last_of("/") + 1, fileName.length() - fileName.find_last_of("/") - 1);

	// ������� ����������, ����� ������� �� ����, � �����
	pathFolderWithFileName = pathFolderWithFileName.substr(0, pathFolderWithFileName.length() - 4);

	// ���� ������ ���� ������ ������
	MyFile output;


	std::string outputFolderPathWithFileName = (outputFolderPath + fileName.substr(fileName.find_last_of('/') + 1));
	outputFolderPathWithFileName = outputFolderPathWithFileName.substr(0, outputFolderPathWithFileName.find_last_of('.'));

	// �������� �����
#ifndef LINUX
	// �������� ������� ����������
	DWORD dwAttrib = GetFileAttributes(outputFolderPathWithFileName.c_str());

	if (dwAttrib == INVALID_FILE_ATTRIBUTES)
	{
		int error = CreateDirectory(outputFolderPathWithFileName.c_str(), NULL);
		
		if (error == ERROR_ALREADY_EXISTS)
		{
#ifdef MY_DEBUG
			std::clog << "Directory is exist\n";
#endif
		}
		else if (error == ERROR_PATH_NOT_FOUND)
		{
#ifdef MY_DEBUG
			std::clog << "Directory is exist\n";
#endif
		}
		else if (error == 0)
		{
			std::cerr << "Exception \"Folder not created\" "
				"void ImgUnpack(" << fileName << ", " << outputFolderPath << ")\n";

			throw "Folder not created";
		}
	}
#else
		system("mkdir " + folderName);
#endif
	

	unsigned int	offset(0),
					size(0),
					numberOfFiles(0);

	// ������ ���� ������������ ������ 24-�� ���������
	char outputFileName[25];

	numberOfFiles = input.readUInt();

	output.createData(134217728);

	// ������ ������ �� ����� input �� ������ ��������� � ������ � output
	for (unsigned int i(0); i < numberOfFiles; ++i)
	{
		// ���� � ������ ����� � i-�� �����
		input.goToByte((unsigned long long)i * 32 + 8);

		offset = input.readUInt();
		size = input.readUShort();

		input.skipBytes(2);
		input.readUCharP(outputFileName, 24);
		input.goToByte(offset * 2048ULL);

		// �������� �� ��������
		// Next line - is work
		//output.createData(size * 2048ULL);

		// ������� ��� ������
		// ������ ����������� �������� ������� � ���
		// �� ���������� ������ � ������� - 127 �� (���� ����� ����� � ������ RenderWare)
		output.goToByte(0);
		output.getSize() = size * 2048ULL;
		// ====

		output.writeUCharP(input.getPointerData(input.getIndex()), size * 2048ULL);
		output.copyDataToFile(pathFolderWithFileName + '/' + outputFileName);
	}

#ifdef MY_DEBUG
	std::clog << "void ImgUnpack(" << fileName << ", " << outputFolderPath << ")\n";
#endif
}



void ImgPack(std::string inputFolderPath, std::string outputFolderPath)
{
	// ������� ���� ������
	MyFile output;

	output.createData(8);

	// ����� ���������
	char signature[5] = "VER2";

	output.writeUCharP(signature, 4);



	// ������� ���������� ������
	unsigned int numberOfFiles(0);

	// �������� ������� ����������
#ifndef LINUX
	DWORD dwAttrib = GetFileAttributes(outputFolderPath.c_str());

	if ((dwAttrib == INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
	{
		std::cerr << "Exception \"Error finding folder\" "
			"in void ImgPack(" << inputFolderPath << ", " << outputFolderPath << ")\n";

		throw "Error finding folder";
	}

	FileStructureToSearch* fileStructuresToSearch;
	unsigned long long generalSize(0);

	{
		// ������ ����������� ������ � ������
		WIN32_FIND_DATA wfd;

		HANDLE h(INVALID_HANDLE_VALUE);

		h = FindFirstFile((inputFolderPath + (inputFolderPath.rfind("*") == std::string::npos ? "*" : "")).c_str(), &wfd);

		if (h == INVALID_HANDLE_VALUE)
		{
			std::cerr << "Exception \"The folder is empty\" "
				"in void ImgPack(" << inputFolderPath << ", " << outputFolderPath << ")\n";

			throw "The folder is empty";
		}

		do
		{
			if (!strcmp(wfd.cFileName, ".") || !strcmp(wfd.cFileName, ".."))
			{
				continue;
			}

			if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				std::cerr << "Exception \"There is a folder in the folder\" "
					"in void ImgPack(" << inputFolderPath << ", " << outputFolderPath << ")\n";

				throw "There is a folder in the folder";
			}

			numberOfFiles++;
		}while(FindNextFile(h, &wfd));

		FindClose(h);

		// ������������� ������� ���� ����:��������
		fileStructuresToSearch = new FileStructureToSearch[numberOfFiles];

		unsigned int index(0);

		h = FindFirstFile((inputFolderPath + (inputFolderPath.rfind("*") == std::string::npos ? "*" : "")).c_str(), &wfd);

		do
		{
			if (!strcmp(wfd.cFileName, ".") || !strcmp(wfd.cFileName, ".."))
			{
				continue;
			}

			if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				std::cerr << "Exception \"There is a folder in the folder\" "
					"in void ImgPack(" << inputFolderPath << ", " << outputFolderPath << ")\n";

				throw "There is a folder in the folder";
			}

			// ��������� ������ ����� � ���������� ��� � ������
			// ������ ������������ ������� ����:
			// size = (wfd.nFileSizeHigh * 0x0000000100000000) + wfd.nFileSizeLow;
			// �� ����� � img ������ � ������ �� ��������� ������ 127 ��

			fileStructuresToSearch[index].m_size = wfd.nFileSizeLow % 2048 ? (unsigned short)(wfd.nFileSizeLow / 2048 + 1) : (unsigned short)(wfd.nFileSizeLow / 2048);
			strcpy_s(fileStructuresToSearch[index].m_name, wfd.cFileName);

			generalSize += fileStructuresToSearch[index].m_size;

			++index;
		}while(FindNextFile(h, &wfd));

		FindClose(h);
	}
#else
	



#endif

	// ����� ���������� ������
	output.writeUInt(numberOfFiles);

	// ����� � ������
	unsigned int blockOffset(numberOfFiles * 32 + 8);
	blockOffset = blockOffset % 2048 ? (unsigned short)(blockOffset / 2048 + 1) : (unsigned short)(blockOffset / 2048);

	// �������� ����� ��� ������ � ��� ������ ������
	output.insertBytes(generalSize * 2048 + blockOffset * 2048 - 8); // "- 8" �.�. 8 ���� ��� �������� � ����

	MyFile input;

	for (int i(0); i < numberOfFiles; ++i)
	{
		// ���� � ������ ����� � i-�� �����
		output.goToByte((unsigned long long)i * 32 + 8);
		
		// ����� � ������
		output.writeUInt(blockOffset);
		output.writeUShort(fileStructuresToSearch[i].m_size);
		output.writeUShort(0); // ������ 0
		output.writeUCharP(fileStructuresToSearch[i].m_name, 24);
		output.goToByte(blockOffset * 2048ULL);

		// ������ ���� � ����
		input.copyDataFromFile(inputFolderPath + fileStructuresToSearch[i].m_name);

		output.writeUCharP(input.getPointerData(), input.getSize());

		blockOffset += fileStructuresToSearch[i].m_size;
	}

	inputFolderPath.pop_back();

	output.copyDataToFile(outputFolderPath + inputFolderPath.substr(inputFolderPath.rfind("/") + 1) + ".img");

	if (fileStructuresToSearch != nullptr)
	{
		delete[] fileStructuresToSearch; // ������ � ���������
	}
#ifdef MY_DEBUG
	std::clog << "void ImgPack(" << inputFolderPath << ", " << outputFolderPath << ")\n";
#endif
}



void TxdUnpack(std::string, std::string)
{

}