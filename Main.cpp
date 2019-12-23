#include "Interface.hpp"


// UNPACK GTA3; Debug; x86; MY_DEBUG = TRUE; STREAM;         3:16
// UNPACK GTA3; Debug; x86; MY_DEBUG = FALSE; STREAM;        1:45
// UNPACK GTA3; Debug; x86; MY_DEBUG = FALSE; NOT STREAM;    1:32
// UNPACK GTA3; Release; x86; MY_DEBUG = FALSE; NOT STREAM;  1:36
// UNPACK GTA3; Release; x86; MY_DEBUG = FALSE; STREAM;      1:42

// Максимальный размер элемента, подфайла, в img архиве равен 127 Мб
// По расчетам (возможно кривым) максимальный размер файла формата img,
// поддерживаемый игровым движком, равен 8191 Гб + 127 Мб

// !!ПЕРЕБРОСИТЬ КЛАСС MyFile В ДРУГОЙ ПРОЕКТ И ПРИСОЕДИНИТЬ ЕГО К ЭТОМУ ПРОЕКТУ

// img unpack D:\Projects\cpp\GnomeSATools\img\cutscene.img -O D:\Projects\cpp\GnomeSATools
// img pack D:\Projects\cpp\GnomeSATools\cutscene -O D:\Projects\cpp\GnomeSATools
int main(int argc, char** argv)
{
	Interface interface(argc, argv);
	return 0;
}