#include "Interface.hpp"
#include "Tools.hpp"

Interface::Interface(int argc, char** argv) :
	m_tool(255), m_command(255), m_numberOfFiles(0), m_inputPaths(nullptr), m_outputFolderPath(""), m_numberOfParams(0)
{
	if (argc < 4)
	{
#ifdef DEBUG
		std::cerr << "Exception \"Number of arguments less than 4\" "
			"in Interface::Interface(" << argc << ", [PROGRAM ARGUMENTS])\n";
#endif

		throw "Number of arguments less than 4";
	}


	// ��������� ������ ��� �����
	std::string tmp;


	// ��� ��������� � ������ �������
	// ����������� �� backslash
	for (int i = 0; i < argc; ++i)
	{
		int j(0);

		while (argv[i][j])
		{
			argv[i][j] = tolower(argv[i][j]);

			if (argv[i][j] == '\\')
			{
				argv[i][j] = '/';
			}

			++j;
		}
	}


	// ��������� ������ ���� ����������
	// m_tool  �  m_command
	for (int i = 1; i < 3; ++i)
	{
		tmp = argv[i];

		if (i == 1)
		{
			if (tmp == "img")
			{
				m_tool = IMG;
			}
			else if (tmp == "txd")
			{
				m_tool = TXD;
			}
			else
			{
#ifdef DEBUG
				std::cerr << "Exception \"No such tool\" "
					"in Interface::Interface(" << argc << ", [PROGRAM ARGUMENTS])\n";
#endif

				throw "No such tool";
			}
		}
		else if (i == 2)
		{
			if (tmp == "pack")
			{
				m_command = PACK;
			}
			else if (tmp == "unpack")
			{
				m_command = UNPACK;
			}
			else
			{
#ifdef DEBUG
				std::cerr << "Exception \"No such command\" "
					"in Interface::Interface(" << argc << ", [PROGRAM ARGUMENTS])\n";
#endif
				throw "No such command";
			}
		}
	}



	for (int i(3); i < argc; ++i)
	{
		// ���������� ���������� ���������� � ���������� ���������
		if (argv[i][0] == '-')
		{
			++m_numberOfParams;
		}
	}

	// �������� �� ������ ���-�� ��������� ������ 3 � �� ����� ���������
	m_numberOfFiles = argc - 3 - m_numberOfParams * 2;

	// ������ � ������� ������� ������
	m_inputPaths = new std::string[m_numberOfFiles];



	m_numberOfParams = 0;

	// ��������� ������� ������
	// ����� ������ ���������� ������ ���������
	for (int i = 3; i < argc; ++i)
	{
		tmp = argv[i];


		// ��������� ����������
		if (tmp == "-o")
		{
			++m_numberOfParams;

			// ����� ��������� �������� ��� ��������
			if (i + 1 >= argc)
			{
#ifdef DEBUG
				std::cerr << "Exception \"No input folder path entered\" "
					"in Interface::Interface(" << argc << ", [PROGRAM ARGUMENTS])\n";
#endif
				throw "No input folder path entered";
			}
			++i;

			tmp = argv[i];
			tmp += (tmp.find_last_of("/") == tmp.length() - 1) ? "" : "/";

			m_outputFolderPath = tmp;

			continue;
		}


		if ((tmp.rfind(".img") == std::string::npos) &&
			(tmp.rfind(".txd") == std::string::npos))
		{
			if (m_command == UNPACK)
			{
				// ������ ���� ����� ��� �� ������ ��������� � ������� ����� (exit(1);) �� ��������� !!!!!!
#ifdef DEBUG
				std::cerr << "Exception \"Invalid file extension\" "
					"in Interface::Interface(" << argc << ", [PROGRAM ARGUMENTS])\n";
#endif
				throw "Invalid file extension";
			}
			else if (m_command == PACK)
			{
				tmp += (tmp.find_last_of("/") == tmp.length() - 1) ? "" : "/";
			}
		}

		m_inputPaths[i - 3 - m_numberOfParams * 2] = tmp;
	}



	// ����� ��������������� �������
	for (int i = 0; i < m_numberOfFiles; ++i)
	{
		if (m_tool == IMG)
		{
			if (m_command == UNPACK)
			{
				ImgUnpack(m_inputPaths[i], m_outputFolderPath);
			}
			else if (m_command == PACK)
			{
				ImgPack(m_inputPaths[i], m_outputFolderPath);
			}
		}
		else if (m_tool == TXD)
		{
			if (m_command == UNPACK)
			{

			}
			else if (m_command == PACK)
			{

			}
		}
	}

#ifdef DEBUG
	std::clog << "Interface::Interface(" << argc << ", [PROGRAM ARGUMENTS])\n";
#endif
}

Interface::~Interface()
{
	delete[] m_inputPaths;
}