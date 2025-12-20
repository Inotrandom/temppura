#ifndef H_CONFIGURATION
#define H_CONFIGURATION

#include <string>

struct configuration_t
{
	std::string begin_comment;
	std::string end_comment;

	std::string default_fextension;
};

#endif