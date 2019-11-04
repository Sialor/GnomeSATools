#include "Tools.hpp"

#include "MyFile.hpp"


/* IMG STRUCT

4 byte - CHAR[4] Версия
4 byte - DWORD Количество файлов в архиве

от 0 до Количество файлов в архиве:
	4 Сдвиг (измеряется в блоках)
	2 Размер потока (измеряется в блоках)
	2 Размер в архиве (измеряется в блоках) Всегда 0
	24 Имя файла
*/


void ImgUnpack(std::string fileName, std::string outputFolderPath)
{
	// Файл который распаковываем
	MyFile input(fileName);

	// Чтение 4 байт, там версия файла
	char signature[5];

	input.readUCharP(signature, 4);

	// Проверка версии
	if (!strcmp(signature, "VER2"))
	{
		throw "Signature mismatch";
	}


	// Создание имени папки
	std::string pathFolderWithFileName;

	pathFolderWithFileName = outputFolderPath + fileName.substr(fileName.find_last_of("/") + 1, fileName.length() - fileName.find_last_of("/") - 1);

	// Убираем расширение, чтобы создать не файл, а папку
	pathFolderWithFileName = pathFolderWithFileName.substr(0, pathFolderWithFileName.length() - 4);

	// Файл вывода всех файлов архива
	MyFile output;


	// Создание папки
#ifndef LINUX
	// Возможно ошибка в количестве символов
	//system((std::string("mkdir ") + fileName.substr(0, start) + folderName).c_str());

	// Проверка наличия директории
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

	// Движок игры ограничивает размер 24-мя символами
	char outputFileName[25];

	numberOfFiles = input.readUInt();

	// Чтение данных из файла input на основе структуры и запись в output
	for (unsigned int i(0); i < numberOfFiles; ++i)
	{
		// Идем к голове файла к i-му файлу
		input.goToByte((unsigned long long)i * 32 + 8);

		// Читаем данные файла
		offset = input.readUInt();
		size = input.readUShort();

		input.skipBytes(2);

		input.readUCharP(outputFileName, 24);

		// Здесь запись данных файла на HDD
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
	// Массив с именами файлов
	std::string *inputFileNames(nullptr);

	// Создать файл вывода
	MyFile output;

	output.createData(8);

	// Пишем сигнатуру
	char signature[5] = "VER2";

	output.writeUCharP(signature, 4);



	// Проверка наличия директории
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

	// Собрать количество файлов
	unsigned int numberOfFiles(0);

	// Имена в одной строке
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

		// В строку добавляем имена файлов
		inputFileNamesStr += std::string(wfd.cFileName) + "/";
		
	}while(FindNextFile(h, &wfd));

	FindClose(h);



#else




#endif



	// Собираем имена ВСЕХ файлов в std::string[кол-во файлов]
	inputFileNames = new std::string[numberOfFiles];
	
	// Парсим filesName и пихаем имена в массив
	for (int i(0); i < numberOfFiles; ++i)
	{
		inputFileNamesStr.pop_back();
		inputFileNames[i] = inputFileNamesStr.substr(inputFileNamesStr.rfind('/') + 1);
		inputFileNamesStr = inputFileNamesStr.substr(0, inputFileNamesStr.rfind('/') + 1);
	}

	// Пишем количество файлов
	output.writeUInt(numberOfFiles);

	// Добавили байты для дерева файлов
	output.insertBytes(32 * (unsigned long long)numberOfFiles);

	MyFile input;


	// Пишем размер i-го файла в байтах
	unsigned int size(0);

	// Пишем размер i-го файла в блоках
	unsigned int blockSize(0);

	// Запоминаем размер файла(offset) - это и будет сдвиг
	unsigned int offset(0);

	for (int i(0); i < numberOfFiles; ++i)
	{
		std::string tmpFileName = inputFolderPath + inputFileNames[i];

		size = MyFile::getSizeFile(tmpFileName);
		blockSize = size % 2048 ? (unsigned short)(size / 2048 + 1) : (unsigned short)(size / 2048);
		offset = output.getSize();

		// Идем к голове файла к i-му файлу
		output.goToByte((unsigned long long)i * 32 + 8);
		
		output.writeUInt(offset);

		// Пишем в блоках
		output.writeUShort(blockSize);

		// Всегда 0
		output.writeUShort(0);

		// Возможно здесь исключение !!!!! (НО НАВРЯТЛИ)
		output.goToByte(offset);

		output.insertBytes(blockSize * 2048ULL);

		// Качаем файл в кучу
		input.copyDataFromFile(tmpFileName);

		// Здесь запись данных файла на HDD
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