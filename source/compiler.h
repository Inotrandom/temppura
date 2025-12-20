#ifndef H_COMPILER
#define H_COMPILER

#include <string>
#include <vector>

#include "lexicon.h"
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

	void build_project(std::string parent_dir);

	void err(const std::string what, std::uint64_t line_n = 0)
	{
		std::cout << "\033[31m[error] A critical compiler error has occurred" << std::endl;
		std::cout << "\t" << what << std::endl << "\033[0m";

		if (line_n > 0)
		{
			std::cout << "Traceback to line " << line_n << std::endl;
		}

		m_running = false;
	}

	void handle_token(std::vector<std::string> &tokens, TOKEN id, std::string &res, std::uint64_t line_n);

	void func_define(std::vector<std::string> tokens, std::string &res, std::uint64_t line_n);

	void sort_aliases_length();

	void fill_aliases(std::string &res);

	void clear_cache() { m_aliases.clear(); }

private:
	void compile_project();

	void clean_project();

	bool m_running;

	std::vector<alias_t> m_aliases;
};

#endif