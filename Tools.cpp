#include "Tools.hpp"

#include "MyFile.hpp"


/* IMG STRUCT

4 byte - CHAR[4] ������
4 byte - DWORD ���������� ������ � ������

�� 0 �� ���������� ������ � ������:
	4 ����� (���������� � ������)
	2 ������ ������ (���������� � ������)
	2 ������ � ������ (���������� � ������) ������ 0
	24 ��� �����
*/


void ImgUnpack(std::string fileName, std::string outputFolderPath)
{
	// ���� ������� �������������
	MyFile input(fileName);

	// ������ 4 ����, ��� ������ �����
	char signature[5];

	input.readUCharP(signature, 4);

	// �������� ������
	if (!strcmp(signature, "VER2"))
	{
		throw "Signature mismatch";
	}


	// �������� ����� �����
	std::string pathFolderWithFileName;

	pathFolderWithFileName = outputFolderPath + fileName.substr(fileName.find_last_of("/") + 1, fileName.length() - fileName.find_last_of("/") - 1);

	// ������� ����������, ����� ������� �� ����, � �����
	pathFolderWithFileName = pathFolderWithFileName.substr(0, pathFolderWithFileName.length() - 4);

	// ���� ������ ���� ������ ������
	MyFile output;


	// �������� �����
#ifndef LINUX
	// �������� ������ � ���������� ��������
	//system((std::string("mkdir ") + fileName.substr(0, start) + folderName).c_str());

	// �������� ������� ����������
	DWORD dwAttrib = GetFileAttributes(outputFolderPath.c_str());

	if ((dwAttrib == INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
	{
		if (CreateDirectory(pathFolderWithFileName.c_str(), NULL) == 0)
		{
#ifdef DEBUG
			std::cerr << "Exception \"Folder not created\" "
				"void ImgUnpack(" << fileName << ", " << outputFolderPath << ")\n";
#endif
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

	// ������ ������ �� ����� input �� ������ ��������� � ������ � output
	for (unsigned int i(0); i < numberOfFiles; ++i)
	{
		// ���� � ������ ����� � i-�� �����
		input.goToByte((unsigned long long)i * 32 + 8);

		// ������ ������ �����
		offset = input.readUInt();
		size = input.readUShort();

		input.skipBytes(2);

		input.readUCharP(outputFileName, 24);

		// ����� ������ ������ ����� �� HDD
		char *fileData = new char[size * 2048ULL];

		input.goToByte(offset * 2048ULL);
		input.readUCharP(fileData, size * 2048ULL);

		output.createData(size * 2048ULL);
		output.writeUCharP(fileData, size * 2048ULL);
		output.copyDataToFile(pathFolderWithFileName + '/' + outputFileName);
	}

#ifdef DEBUG
	std::clog << "void ImgUnpack(" << fileName << ", " << outputFolderPath << ")\n";
#endif
}



void ImgPack(std::string inputFolderPath, std::string outputFolderPath)
{
	// ������ � ������� ������
	std::string *inputFileNames(nullptr);

	// ������� ���� ������
	MyFile output;

	output.createData(8);

	// ����� ���������
	char signature[5] = "VER2";

	output.writeUCharP(signature, 4);



	// �������� ������� ����������
#ifndef LINUX
	DWORD dwAttrib = GetFileAttributes(outputFolderPath.c_str());

	if ((dwAttrib == INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
	{
#ifdef DEBUG
		std::cerr << "Exception \"Error finding folder\" "
			"in void ImgPack(" << inputFolderPath << ", " << outputFolderPath << ")\n";
#endif
		throw "Error finding folder";
	}

	// ������� ���������� ������
	unsigned int numberOfFiles(0);

	// ����� � ����� ������
	std::string inputFileNamesStr;

	WIN32_FIND_DATA wfd;

	HANDLE h(INVALID_HANDLE_VALUE);

	h = FindFirstFile((inputFolderPath + (inputFolderPath.rfind("*") == std::string::npos ? "*" : "")).c_str(), &wfd);

	if (h == INVALID_HANDLE_VALUE)
	{
#ifdef DEBUG
		std::cerr << "Exception \"The folder is empty\" "
			"in void ImgPack(" << inputFolderPath << ", " << outputFolderPath << ")\n";
#endif
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
#ifdef DEBUG
			std::cerr << "Exception \"There is a folder in the folder\" "
				"in void ImgPack(" << inputFolderPath << ", " << outputFolderPath << ")\n";
#endif
			throw "There is a folder in the folder";
		}

		numberOfFiles++;

		// � ������ ��������� ����� ������
		inputFileNamesStr += std::string(wfd.cFileName) + "/";
		
	}while(FindNextFile(h, &wfd));

	FindClose(h);



#else




#endif



	// �������� ����� ���� ������ � std::string[���-�� ������]
	inputFileNames = new std::string[numberOfFiles];
	
	// ������ filesName � ������ ����� � ������
	for (int i(0); i < numberOfFiles; ++i)
	{
		inputFileNamesStr.pop_back();
		inputFileNames[i] = inputFileNamesStr.substr(inputFileNamesStr.rfind('/') + 1);
		inputFileNamesStr = inputFileNamesStr.substr(0, inputFileNamesStr.rfind('/') + 1);
	}

	// ����� ���������� ������
	output.writeUInt(numberOfFiles);

	// �������� ����� ��� ������ ������
	output.insertBytes(32 * (unsigned long long)numberOfFiles);

	MyFile input;


	// ����� ������ i-�� ����� � ������
	unsigned int size(0);

	// ����� ������ i-�� ����� � ������
	unsigned int blockSize(0);

	// ���������� ������ �����(offset) - ��� � ����� �����
	unsigned int offset(0);

	for (int i(0); i < numberOfFiles; ++i)
	{
		std::string tmpFileName = inputFolderPath + inputFileNames[i];

		size = MyFile::getSizeFile(tmpFileName);
		blockSize = size % 2048 ? (unsigned short)(size / 2048 + 1) : (unsigned short)(size / 2048);
		offset = output.getSize();

		// ���� � ������ ����� � i-�� �����
		output.goToByte((unsigned long long)i * 32 + 8);
		
		output.writeUInt(offset);

		// ����� � ������
		output.writeUShort(blockSize);

		// ������ 0
		output.writeUShort(0);

		// �������� ����� ���������� !!!!! (�� ��������)
		output.goToByte(offset);

		output.insertBytes(blockSize * 2048ULL);

		// ������ ���� � ����
		input.copyDataFromFile(tmpFileName);

		// ����� ������ ������ ����� �� HDD
		char *fileData = new char[size];

		input.readUCharP(fileData, size);

		output.writeUCharP(fileData, size);
	}

	inputFolderPath.pop_back();

	output.copyDataToFile(outputFolderPath + inputFolderPath.substr(inputFolderPath.rfind("/") + 1) + ".img");

#ifdef DEBUG
	std::clog << "void ImgPack(" << inputFolderPath << ", " << outputFolderPath << ")\n";
#endif
}