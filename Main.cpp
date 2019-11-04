#include "Interface.hpp"

// img unpack D:\Projects\cpp\GnomeSATools\img\cutscene.img -O D:\Projects\cpp\GnomeSATools
// img pack D:\Projects\cpp\GnomeSATools\cutscene -O D:\Projects\cpp\GnomeSATools
int main(int argc, char** argv)
{
	Interface interface(argc, argv);

	return 0;
}



// Исправлена утечка памяти. Исрпавлено чтение данных из файлов. Исрпавлен тип данных ввода/вывода Си строк/
// Си строки заменены на std::string
// Добавлена запаковка img архивов
// Поправлены передачи путей и файлов
// Добавлен аргумент -o для вывода в определенную директорию