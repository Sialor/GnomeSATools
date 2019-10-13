#include "Interface.hpp"



Interface::Interface(int argc, char** argv) : 
	m_tool(255), m_command(255), m_countFiles(0), m_fileNames(nullptr)
{
	if (argc < 4)
	{
#ifdef DEBUG
		std::cerr << "Exception \"Number of arguments less than 4\" "
			"in Interface::Interface(" << argc << ", " << argv << ")\n";
#endif

		throw "Number of arguments less than 4";
	}

	// Обработка первых двух аргументов
	// m_tool  и  m_command
	{
		std::string tmp;
		int j(0);

		for (int i = 1; i < 3; ++i)
		{
			tmp = argv[i];
			j = 0;
			
			while (tmp[j] != '\0')
			{
				tmp[j] = toupper(tmp[j]);
				++j;
			}

			if (i == 1)
			{
				if (tmp == "IMG")
				{
					m_tool = IMG;
				}
				else if (tmp == "TXD")
				{
					m_tool = TXD;
				}
				else
				{
#ifdef DEBUG
					std::cerr << "Exception \"No such tool\" "
						"in Interface::Interface(" << argc << ", " << argv << ")\n";
#endif

					throw "No such tool";
				}
			}
			else if (i == 2)
			{
				if (tmp == "PACK")
				{
					m_command = PACK;
				}
				else if (tmp == "UNPACK")
				{
					m_command = UNPACK;
				}
				else
				{
#ifdef DEBUG
					std::cerr << "Exception \"No such command\" "
						"in Interface::Interface(" << argc << ", " << argv << ")\n";
#endif

					throw "No such command";
				}
			}
		}

		m_fileNames = new std::string[argc - 3];

		// Обработка остальных аргументов
		// Имена файлов необходимо обдать обработке
		for (int i = 3; i < argc; ++i)
		{
			tmp = argv[i];
			j = 0;

			while (tmp[j] != '\0')
			{
				tmp[j] = toupper(tmp[j]);
				++j;
			}

			m_fileNames[i - 3] = tmp;
		}
	}

#ifdef DEBUG
	std::clog << "Interface::Interface(" << argc << ", " << argv << ")\n";
#endif
}

Interface::~Interface()
{
	delete[] m_fileNames;
}