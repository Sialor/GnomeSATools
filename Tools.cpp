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


bool MyFile::m_isNotStream(false);

void ImgUnpack(std::string fileName, std::string outputFolderPath)
{
	// ���� ������� �������������
	MyFile input(fileName);

	// ������ 4 ����, ��� ������ �����
	char signature[5];

	input.readUCharP(signature, 4);

	// �������� ������
	if (std::string(signature) != "VER2")
	{
#ifdef MY_DEBUG
		std::cerr << "Exception \"Signature mismatch\" "
			"void ImgUnpack(" << fileName << ", " << outputFolderPath << ")\n";
#endif

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
		if (CreateDirectory(outputFolderPathWithFileName.c_str(), NULL) == 0)
		{
#ifdef MY_DEBUG
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
	// ������ � ������� ������
	std::string *inputFileNames(nullptr);

	// ������� ���� ������
	MyFile output;

	output.createData(8);

	// ����� ���������
	char signature[5] = "VER2";

	output.writeUCharP(signature, 4);



	// ������� ���������� ������
	unsigned int numberOfFiles(0);

	// ����� � ����� ������
	std::string inputFileNamesStr;

	unsigned int *arraySizeInBlocks;

	// �������� ������� ����������
#ifndef LINUX
	DWORD dwAttrib = GetFileAttributes(outputFolderPath.c_str());

	if ((dwAttrib == INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
	{
#ifdef MY_DEBUG
		std::cerr << "Exception \"Error finding folder\" "
			"in void ImgPack(" << inputFolderPath << ", " << outputFolderPath << ")\n";
#endif
		throw "Error finding folder";
	}

	WIN32_FIND_DATA wfd;

	HANDLE h(INVALID_HANDLE_VALUE);

	h = FindFirstFile((inputFolderPath + (inputFolderPath.rfind("*") == std::string::npos ? "*" : "")).c_str(), &wfd);

	if (h == INVALID_HANDLE_VALUE)
	{
#ifdef MY_DEBUG
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
#ifdef MY_DEBUG
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

	arraySizeInBlocks = new unsigned int[numberOfFiles];


	// ������ ����������� ������ � ������
	h = FindFirstFile((inputFolderPath + (inputFolderPath.rfind("*") == std::string::npos ? "*" : "")).c_str(), &wfd);

	if (h == INVALID_HANDLE_VALUE)
	{
#ifdef MY_DEBUG
		std::cerr << "Exception \"The folder is empty\" "
			"in void ImgPack(" << inputFolderPath << ", " << outputFolderPath << ")\n";
#endif
		throw "The folder is empty";
	}

	{
		unsigned int index(0);
		unsigned int size(0);

		do
		{
			if (!strcmp(wfd.cFileName, ".") || !strcmp(wfd.cFileName, ".."))
			{
				continue;
			}

			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
#ifdef MY_DEBUG
				std::cerr << "Exception \"There is a folder in the folder\" "
					"in void ImgPack(" << inputFolderPath << ", " << outputFolderPath << ")\n";
#endif
				throw "There is a folder in the folder";
			}

			// ��������� ������ ����� � ���������� ��� � ������
			// ������ ������������ ������� ����:
			// size = (wfd.nFileSizeHigh * 0x0000000100000000) + wfd.nFileSizeLow;
			// �� ����� � img ������ � ������ �� ��������� ������ 127 ��

			size = wfd.nFileSizeLow;

			arraySizeInBlocks[index] = size % 2048 ? (unsigned short)(size / 2048 + 1) : (unsigned short)(size / 2048);

			++index;
		} while (FindNextFile(h, &wfd));
	}



#else
	// ���������� inputFileNamesStr, numberOfFiles ��� ��������� ���� �� ����� ������������
	// � ������ ����� ���� ���������� ��������� ���������� ������, ��� �� � ���������� inputFileNameStr
	// �������� �������� ������ ������� �� '/'



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
	
	{
		unsigned long long generalSize(0);

		for (int i(0); i < numberOfFiles; ++i)
		{
			generalSize += arraySizeInBlocks[i];
		}

		// �������� ����� ��� ������
		output.insertBytes(generalSize * 2048);
	}

	MyFile input;

	unsigned int size(0);

	// ����� � ������
	unsigned int blockOffset(numberOfFiles * 32 + 8);
	blockOffset = blockOffset % 2048 ? (unsigned short)(blockOffset / 2048 + 1) : (unsigned short)(blockOffset / 2048);

	std::string tmpFileName;

	for (int i(0); i < numberOfFiles; ++i)
	{
		tmpFileName = inputFolderPath + inputFileNames[i];

		size = MyFile::getSizeFile(tmpFileName);
		
		// ���� � ������ ����� � i-�� �����
		output.goToByte((unsigned long long)i * 32 + 8);
		
		// ����� � ������
		output.writeUInt(blockOffset);
		output.writeUShort(arraySizeInBlocks[i]);
		output.writeUShort(0); // ������ 0
		output.goToByte(blockOffset * 2048ULL);

		// ������ ���� � ����
		input.copyDataFromFile(tmpFileName);

		output.writeUCharP(input.getPointerData(), size);

		blockOffset += arraySizeInBlocks[i];
	}

	inputFolderPath.pop_back();

	output.copyDataToFile(outputFolderPath + inputFolderPath.substr(inputFolderPath.rfind("/") + 1) + ".img");

	if (inputFileNames != nullptr)
	{
		delete[] inputFileNames;
	}

	if (arraySizeInBlocks != nullptr)
	{
		delete[] arraySizeInBlocks; // ������ � ��������� ������ � ������
	}
#ifdef MY_DEBUG
	std::clog << "void ImgPack(" << inputFolderPath << ", " << outputFolderPath << ")\n";
#endif
}