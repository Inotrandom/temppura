#ifndef H_COMPILER
#define H_COMPILER

#include <string>
#include <vector>
#include <map>

#include "flag.h"
#include "configuration.h"

#include "tokenizer.h"

#include "alias.h"

class compiler_t
{
public:
	void abort()
	{
		m_running = false;
		std::cout << "Compilation aborted." << std::endl;
	}

	auto compile(std::string script) -> std::string;

	void load_config();

	void load_cache();

	void gen_cache();

	void build_project(std::string parent_dir);

	void err(const std::string what, std::uint64_t line_n = 0)
	{
		std::cout << "\033[31m[error] A critical compiler error has occurred" << std::endl;
		std::cout << "\t" << what << std::endl << "\033[0m";

		if (line_n > 0)
		{
			std::cout << "Traceback to line " << line_n << std::endl;
		}

		abort();
	}

	void handle_token(std::vector<std::string> &tokens, TOKEN id, std::uint64_t line_n);

	void func_define(std::vector<std::string> tokens, std::uint64_t line_n);
	void func_flag(std::vector<std::string> tokens, std::uint64_t line_n);

	void sort_aliases_length();

	void evaluate_aliases(std::string &res);

	void clear_mem_cache()
	{
		m_aliases.clear();
		m_flags.clear();
	}

	auto has_flag(FLAG_TYPE flag) -> bool { return (m_flags.find(flag) != m_flags.end()); }

private:
	auto compile_project() -> std::uint64_t;

	void clean_project();

	bool m_running = true;

	std::vector<alias_t> m_aliases;
	std::map<FLAG_TYPE, std::string> m_flags;

	configuration_t m_config;

	cache_t m_cache;
};

#endif // H_COMPILER