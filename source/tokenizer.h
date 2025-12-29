#ifndef H_TOKENIZER
#define H_TOKENIZER

#include <map>
#include <string>
#include <cstdint>
#include <stdexcept>

#include "lexicon.h"

enum class TOKEN : uint8_t
{
	NONEXISTENT,
	DEFINE,
	FLAG
};

// clang-format off
const std::map<std::string, TOKEN> tokenizer = {
	{LEXICON::DEFINE, TOKEN::DEFINE},
    {LEXICON::FLAG, TOKEN::FLAG}
};
// clang-format on

inline auto tokenize(const std::string what) -> TOKEN
{
	TOKEN res = TOKEN::NONEXISTENT;

	try
	{
		res = tokenizer.at(what);
	}
	catch (std::out_of_range &e)
	{
		return TOKEN::NONEXISTENT;
	}

	return res;
}

#endif // H_TOKENIZER