#ifndef H_CONFIGURATION
#define H_CONFIGURATION

#include <string>
#include <cstdint>
#include <sstream>

#include "strutils.h"

struct cache_t
{
	std::string build_number = "0";
};

namespace CACHE_LEXICON
{
const std::string BUILD_NUMBER = "_BN";
}

struct configuration_t
{
	std::string begin_comment;
	std::string end_comment;

	std::string default_fextension;
	std::string project_name;
};

namespace CONFIG_LEXICON
{
const std::string STATEMENT_DELIM = ";";
const std::string KP_DELIM = "=";

const std::string BEGIN_COMMENT = "BEGIN_COMMENT";
const std::string END_COMMENT = "END_COMMENT";
const std::string DEFAULT_FEXTENSION = "DEFAULT_FILE_EXTENSION";
const std::string PROJECT_NAME = "PROJECT_NAME";
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

inline auto produce_pair(const std::string k, const std::string p) -> std::string
{
	std::stringstream res;
	res << k << CONFIG_LEXICON::KP_DELIM << p << CONFIG_LEXICON::STATEMENT_DELIM;
	return res.str();
};

#endif // H_CONFIGURATION