#include "Interface.hpp"


// UNPACK GTA3; Debug; x86; MY_DEBUG = TRUE; STREAM;         3:16
// UNPACK GTA3; Debug; x86; MY_DEBUG = FALSE; STREAM;        1:45
// UNPACK GTA3; Debug; x86; MY_DEBUG = FALSE; NOT STREAM;    1:32
// UNPACK GTA3; Release; x86; MY_DEBUG = FALSE; NOT STREAM;  1:36
// UNPACK GTA3; Release; x86; MY_DEBUG = FALSE; STREAM;      1:42

// ������������ ������ ��������, ��������, � img ������ ����� 127 ��
// �� �������� (�������� ������) ������������ ������ ����� ������� img,
// �������������� ������� �������, ����� 8191 �� + 127 ��

// !!����������� ����� MyFile � ������ ������ � ������������ ��� � ����� �������

// img pack "D:\Steam\steamapps\common\Grand Theft Auto San Andreas/unpacked/models\player/" -o "D:\Steam\steamapps\common\Grand Theft Auto San Andreas\models"
// img unpack "D:\Steam\steamapps\common\Grand Theft Auto San Andreas/models\player.img" -o "D:\Steam\steamapps\common\Grand Theft Auto San Andreas\unpacked/models"
int main(int argc, char** argv)
{
	try
	{
		Interface interface(argc, argv);
	}
	catch (...)
	{
		
	}

	return 0;
}