#ifndef H_CONFIGURATION
#define H_CONFIGURATION

#include <string>
#include <cstdint>

#include "strutils.h"

struct configuration_t
{
	std::string begin_comment;
	std::string end_comment;

	std::string default_fextension;
};

namespace CONFIG_LEXICON
{
const std::string STATEMENT_DELIM = ";";
const std::string KP_DELIM = "=";

const std::string BEGIN_COMMENT = "BEGIN_COMMENT";
const std::string END_COMMENT = "END_COMMENT";
const std::string DEFAULT_FEXTENSION = "DEFAULT_FILE_EXTENSION";
} // namespace CONFIG_LEXICON

const std::uint8_t KP_SIZE = 2;
const std::string DEFAULT_STRING_VALUE = "";

inline auto get_pair(const std::string &contents, const std::string k) -> std::string
{
	std::vector<std::string> statements = string_split(contents, CONFIG_LEXICON::STATEMENT_DELIM);

	for (auto statement : statements)
	{
		std::vector<std::string> kp = string_split(statement, CONFIG_LEXICON::KP_DELIM);

		if (kp.size() != KP_SIZE)
			return DEFAULT_STRING_VALUE;

		string_trim(kp[0]);
		string_trim(kp[1]);

		if (kp[0] != k)
			continue;

		return kp[1];
	}

	return DEFAULT_STRING_VALUE;
}

#endif // H_CONFIGURATION