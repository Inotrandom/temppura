#ifndef H_UTILS
#define H_UTILS

#define MILLI_TO_U 1.0 / 1000.0

#include <string>

inline auto ddelim() -> std::string
{
#if defined _WIN32
	return "\\\\";
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	return "/";
#elif defined(__APPLE__)
	return "/";
#endif
}

#endif // H_UTILS