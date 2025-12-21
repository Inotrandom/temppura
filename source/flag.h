#ifndef H_FLAG
#define H_FLAG

#include <map>
#include <string>
#include <stdexcept>
#include <cstdint>

enum class FLAG_TYPE : uint8_t
{
	NONEXISTENT,
	OUTPUT_FILE_EXTENSION
};

// clang-format off
const std::map<std::string, FLAG_TYPE> flag_tokenizer = {
	{"OUTPUT_FILE_EXTENSION", FLAG_TYPE::OUTPUT_FILE_EXTENSION},
};
// clang-format on

inline auto flag_tokenize(const std::string what) -> FLAG_TYPE
{
	FLAG_TYPE res = FLAG_TYPE::NONEXISTENT;

	try
	{
		res = flag_tokenizer.at(what);
	}
	catch (std::out_of_range &e)
	{
		return FLAG_TYPE::NONEXISTENT;
	}

	return res;
}

#endif // H_FLAG