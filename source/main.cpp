#include <iostream>
#include <cstdint>
#include <string>
#include <filesystem>

#include "compiler.h"

static void console_clear()
{
#if defined _WIN32
	system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	system("clear");
#elif defined(__APPLE__)
	system("clear");
#endif
}

auto main() -> int
{
	bool m_running = true;

	compiler_t compiler = compiler_t();

	console_clear();

	while (m_running)
	{
		std::string path = std::string(std::filesystem::current_path());
		std::cout << path << " temppura >";

		std::string buf;
		std::getline(std::cin, buf);

		if (buf == "exit")
		{
			console_clear();
			return 0;
		}

		if (buf == "cd")
		{
			std::getline(std::cin, buf);

			// Incredibly lazy
			if (std::filesystem::exists(buf) == false)
				continue;
			std::filesystem::current_path(buf);

			continue;
		}

		if (buf == "clear")
		{
			console_clear();

			continue;
		}

		compiler.clear_mem_cache();
		compiler.build_project(buf);
	}

	return 0;
}