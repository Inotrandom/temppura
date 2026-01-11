#ifndef H_FILE_UTILS
#define H_FILE_UTILS

#include <fstream>
#include <optional>
#include <string>
#include <sstream>
#include <cstdint>

/**
 * Reads the contents of a file.
 *
 * @param path The relative path from the CWD
 *
 * @return std::nullopt if the file failed to read, or std::string
 * if it was successful
 */
inline auto file_read(const std::string &path) -> std::optional<std::string>
{
	std::ifstream file(path);

	if (file.is_open() == false)
		return std::nullopt;

	std::stringstream buf;
	buf << file.rdbuf();

	file.close();

	return buf.str();
}

inline auto file_write(const std::string &path, const std::string &contents) -> std::int8_t
{
	std::ofstream file = std::ofstream(path);

	if (file.is_open() == false)
		return 1;

	file << contents;

	file.close();

	return 0;
}

#endif // H_FILE_UTILS