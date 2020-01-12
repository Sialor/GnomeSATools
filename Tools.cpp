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
	char signature[5] = "";

	input.readUCharP(signature, 4);

	// Проверка версии
	if (std::string(signature) != "VER2")
	{
		std::cerr << "Exception \"Signature mismatch\" "
			"void ImgUnpack(" << fileName << ", " << outputFolderPath << ")\n";

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
	// Создать файл вывода
	MyFile output;

	output.createData(8);

	// Пишем сигнатуру
	char signature[5] = "VER2";

	output.writeUCharP(signature, 4);



	// Собрать количество файлов
	unsigned int numberOfFiles(0);

	// Проверка наличия директории
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
		// Запись размерности файлов в массив
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

		// Инициализация массива вида ключ:значение
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

			// Считываем размер файла и записываем его в массив
			// Обычно используется формула ниже:
			// size = (wfd.nFileSizeHigh * 0x0000000100000000) + wfd.nFileSizeLow;
			// но файлы в img архиве в движке не превышают размер 127 мб

			fileStructuresToSearch[index].m_size = wfd.nFileSizeLow % 2048 ? (unsigned short)(wfd.nFileSizeLow / 2048 + 1) : (unsigned short)(wfd.nFileSizeLow / 2048);
			strcpy_s(fileStructuresToSearch[index].m_name, wfd.cFileName);

			generalSize += fileStructuresToSearch[index].m_size;

			++index;
		}while(FindNextFile(h, &wfd));

		FindClose(h);
	}
#else
	



#endif

	// Пишем количество файлов
	output.writeUInt(numberOfFiles);

	// Сдвиг в блоках
	unsigned int blockOffset(numberOfFiles * 32 + 8);
	blockOffset = blockOffset % 2048 ? (unsigned short)(blockOffset / 2048 + 1) : (unsigned short)(blockOffset / 2048);

	// Добавили байты для данных и для дерева файлов
	output.insertBytes(generalSize * 2048 + blockOffset * 2048 - 8); // "- 8" т.к. 8 байт уже записали в файл

	MyFile input;

	for (int i(0); i < numberOfFiles; ++i)
	{
		// Идем к голове файла к i-му файлу
		output.goToByte((unsigned long long)i * 32 + 8);
		
		// Пишем в блоках
		output.writeUInt(blockOffset);
		output.writeUShort(fileStructuresToSearch[i].m_size);
		output.writeUShort(0); // Всегда 0
		output.writeUCharP(fileStructuresToSearch[i].m_name, 24);
		output.goToByte(blockOffset * 2048ULL);

		// Качаем файл в кучу
		input.copyDataFromFile(inputFolderPath + fileStructuresToSearch[i].m_name);

		output.writeUCharP(input.getPointerData(), input.getSize());

		blockOffset += fileStructuresToSearch[i].m_size;
	}

	inputFolderPath.pop_back();

	output.copyDataToFile(outputFolderPath + inputFolderPath.substr(inputFolderPath.rfind("/") + 1) + ".img");

	if (fileStructuresToSearch != nullptr)
	{
		delete[] fileStructuresToSearch; // Массив с алфавитом
	}
#ifdef MY_DEBUG
	std::clog << "void ImgPack(" << inputFolderPath << ", " << outputFolderPath << ")\n";
#endif
}