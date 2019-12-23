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


bool MyFile::m_isNotStream(false);

void ImgUnpack(std::string fileName, std::string outputFolderPath)
{
	// Файл который распаковываем
	MyFile input(fileName);

	// Чтение 4 байт, там версия файла
	char signature[5];

	input.readUCharP(signature, 4);

	// Проверка версии
	if (std::string(signature) != "VER2")
	{
#ifdef MY_DEBUG
		std::cerr << "Exception \"Signature mismatch\" "
			"void ImgUnpack(" << fileName << ", " << outputFolderPath << ")\n";
#endif

		throw "Signature mismatch";
	}

	// Создание имени папки
	std::string pathFolderWithFileName;

	pathFolderWithFileName = outputFolderPath + fileName.substr(fileName.find_last_of("/") + 1, fileName.length() - fileName.find_last_of("/") - 1);

	// Убираем расширение, чтобы создать не файл, а папку
	pathFolderWithFileName = pathFolderWithFileName.substr(0, pathFolderWithFileName.length() - 4);

	// Файл вывода всех файлов архива
	MyFile output;


	std::string outputFolderPathWithFileName = (outputFolderPath + fileName.substr(fileName.find_last_of('/') + 1));
	outputFolderPathWithFileName = outputFolderPathWithFileName.substr(0, outputFolderPathWithFileName.find_last_of('.'));

	// Создание папки
#ifndef LINUX
	// Проверка наличия директории
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

	// Движок игры ограничивает размер 24-мя символами
	char outputFileName[25];

	numberOfFiles = input.readUInt();

	output.createData(134217728);

	// Чтение данных из файла input на основе структуры и запись в output
	for (unsigned int i(0); i < numberOfFiles; ++i)
	{
		// Идем к голове файла к i-му файлу
		input.goToByte((unsigned long long)i * 32 + 8);

		offset = input.readUInt();
		size = input.readUShort();

		input.skipBytes(2);
		input.readUCharP(outputFileName, 24);
		input.goToByte(offset * 2048ULL);

		// Проверка на скорость
		// Next line - is work
		//output.createData(size * 2048ULL);

		// Удалить две строки
		// Вместо постоянного создания области в ОЗУ
		// Мы используем память с запасом - 127 Мб (макс объем файла в архиве RenderWare)
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
	// Массив с именами файлов
	std::string *inputFileNames(nullptr);

	// Создать файл вывода
	MyFile output;

	output.createData(8);

	// Пишем сигнатуру
	char signature[5] = "VER2";

	output.writeUCharP(signature, 4);



	// Собрать количество файлов
	unsigned int numberOfFiles(0);

	// Имена в одной строке
	std::string inputFileNamesStr;

	unsigned int *arraySizeInBlocks;

	// Проверка наличия директории
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

		// В строку добавляем имена файлов
		inputFileNamesStr += std::string(wfd.cFileName) + "/";
	}while(FindNextFile(h, &wfd));

	FindClose(h);

	arraySizeInBlocks = new unsigned int[numberOfFiles];


	// Запись размерности файлов в массив
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

			// Считываем размер файла и записываем его в массив
			// Обычно используется формула ниже:
			// size = (wfd.nFileSizeHigh * 0x0000000100000000) + wfd.nFileSizeLow;
			// но файлы в img архиве в движке не превышают размер 127 мб

			size = wfd.nFileSizeLow;

			arraySizeInBlocks[index] = size % 2048 ? (unsigned short)(size / 2048 + 1) : (unsigned short)(size / 2048);

			++index;
		} while (FindNextFile(h, &wfd));
	}



#else
	// Переменные inputFileNamesStr, numberOfFiles уже объявлены выше их можно использовать
	// В данном блоке кода достаточно посчитать количество файлов, так же в переменную inputFileNameStr
	// запихать названия файлов чередуя их '/'



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
	
	{
		unsigned long long generalSize(0);

		for (int i(0); i < numberOfFiles; ++i)
		{
			generalSize += arraySizeInBlocks[i];
		}

		// Добавили байты для данных
		output.insertBytes(generalSize * 2048);
	}

	MyFile input;

	unsigned int size(0);

	// Сдвиг в блоках
	unsigned int blockOffset(numberOfFiles * 32 + 8);
	blockOffset = blockOffset % 2048 ? (unsigned short)(blockOffset / 2048 + 1) : (unsigned short)(blockOffset / 2048);

	std::string tmpFileName;

	for (int i(0); i < numberOfFiles; ++i)
	{
		tmpFileName = inputFolderPath + inputFileNames[i];

		size = MyFile::getSizeFile(tmpFileName);
		
		// Идем к голове файла к i-му файлу
		output.goToByte((unsigned long long)i * 32 + 8);
		
		// Пишем в блоках
		output.writeUInt(blockOffset);
		output.writeUShort(arraySizeInBlocks[i]);
		output.writeUShort(0); // Всегда 0
		output.goToByte(blockOffset * 2048ULL);

		// Качаем файл в кучу
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
		delete[] arraySizeInBlocks; // Массив с размерами файлов в блоках
	}
#ifdef MY_DEBUG
	std::clog << "void ImgPack(" << inputFolderPath << ", " << outputFolderPath << ")\n";
#endif
}