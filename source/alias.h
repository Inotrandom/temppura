#ifndef H_ALIAS
#define H_ALIAS

#include <string>

struct alias_t
{
	std::string key;
	std::string value;

	alias_t(const std::string k, const std::string v)
	{
		key = k;
		value = v;
	}
};

#endif // H_ALIAS