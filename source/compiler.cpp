#include <cstdint>
#include <iostream>

#include <string>
#include <string.h>

#include <sstream>
#include <filesystem>
#include <algorithm>

#include <chrono>

#include "compiler.h"

#include "strutils.h"
#include "fileutils.h"
#include "utils.h"

const std::string SIGNATURE = "$COMMENT File compiled on $T $ENDCOMMENT\n\n$COMMENT Temppura: https://github.com/Inotrandom/temppura $ENDCOMMENT \n$COMMENT "
							  "12/20/2025 (MIT License) $ENDCOMMENT\n\n$COMMENT THIS FILE IS NOT INTENDED FOR HUMAN MODIFICATION! $ENDCOMMENT\n$COMMENT (it's "
							  "just less readable...) $ENDCOMMENT\n\n";

// clang-format off
#define FORCE_CONFIG(TEST, VAL) if (m_config.TEST == DEFAULT_STRING_VALUE) { std::stringstream msg; msg << "Configuration failed! " << VAL << " is undefined!"; err(msg.str()); return; }
// clang-format on

void compiler_t::load_config()
{
	std::stringstream path;
	path << "./" << DIR::SOURCE << "/" << FILE_NAME::CONFIG;
	std::optional<std::string> contents = file_read(path.str());

	if (contents.has_value() == false)
	{
		std::stringstream msg;
		msg << "No \"" << FILE_NAME::CONFIG << "\" file found in \"" << DIR::SOURCE << "\" directory.";
		err(msg.str());
	}

	std::string opened_contents = contents.value();

	m_config.begin_comment = get_pair(opened_contents, CONFIG_LEXICON::BEGIN_COMMENT);
	FORCE_CONFIG(begin_comment, CONFIG_LEXICON::BEGIN_COMMENT)
	m_config.begin_comment = trim_first_and_last(m_config.begin_comment);

	m_config.end_comment = get_pair(opened_contents, CONFIG_LEXICON::END_COMMENT);
	FORCE_CONFIG(end_comment, CONFIG_LEXICON::END_COMMENT)
	m_config.end_comment = trim_first_and_last(m_config.end_comment);

	m_config.default_fextension = get_pair(opened_contents, CONFIG_LEXICON::DEFAULT_FEXTENSION);
	m_config.default_fextension = trim_first_and_last(m_config.default_fextension);
}

auto compiler_t::compile(std::string script) -> std::string
{
	m_running = true;
	std::string res = script;

	std::vector<std::string> lines = string_split(script, LEXICON::DELIM);

	std::string preproc = LEXICON::PREPROC;
	string_replace(preproc, "$BEGINCOMMENT", m_config.begin_comment);

	std::uint64_t preproc_id_len = preproc.size();

	std::uint64_t line_n = 0;
	for (auto line : lines)
	{
		line_n++;

		if (line.size() == 0)
			continue;

		if (line.substr(0, preproc_id_len) != preproc)
		{
			continue;
		}

		std::vector<std::string> tokens = string_split(line, LEXICON::TOKEN_DELIM);

		if (tokens.size() == 0)
		{
			continue;
		}

		if (tokens[0].length() < preproc_id_len)
		{
			continue;
		}

		std::string shaved_id = tokens[0].substr(preproc_id_len, tokens[0].size());
		if (m_config.end_comment != "")
			string_replace(shaved_id, m_config.end_comment, "");

		TOKEN tokenized_id = tokenize(shaved_id);

		if (tokenized_id == TOKEN::NONEXISTENT)
		{
			std::stringstream msg;
			msg << "Unidentified temppura directive on line " << line_n;
			msg << "\n\t(" << shaved_id << ")";
			err(msg.str());
			continue;
		}

		tokens.erase(tokens.begin()); // Erase the ID
		handle_token(tokens, tokenized_id, line_n);
	}

	fill_aliases(res);

	const auto now = std::chrono::system_clock::now();
	const std::time_t t_c = std::chrono::system_clock::to_time_t(now);

	// This is REALLY freaking gross
	std::string time = std::string(strtok(std::ctime(&t_c), "\n"));

	std::string signature = SIGNATURE;
	std::string time_and_zone = (time);

	string_replace(signature, "$T", time_and_zone);
	string_replace(signature, "$ENDCOMMENT", m_config.end_comment);
	string_replace(signature, "$COMMENT", m_config.begin_comment);

	res = signature + res;

	return res;
}

void compiler_t::handle_token(std::vector<std::string> &tokens, TOKEN id, std::uint64_t line_n)
{
	// NOTE: The `tokens` parameter does not include an ID... this is passed in as the parameter `id`

	// Clean tokens
	for (auto &token : tokens)
	{
		if (m_config.end_comment == "")
			break;

		string_replace(token, m_config.end_comment, "");
	}

	switch (id)
	{
	case (TOKEN::NONEXISTENT):
	{
		// There's nothing to do about a nonexistent token.
		return;
	}

	case (TOKEN::DEFINE):
	{
		func_define(tokens, line_n);
		break;
	}

	case (TOKEN::FLAG):
	{
		func_flag(tokens, line_n);
		break;
	}
	}
}

const std::uint8_t MIN_FLAG_ARGS = 1;
const std::uint8_t OUTPUT_FILE_EXTENSION_ARGS = 2;

void compiler_t::func_flag(std::vector<std::string> tokens, std::uint64_t line_n)
{
	if (tokens.size() < MIN_FLAG_ARGS)
	{
		err("Incorrect arguments to flag directive", line_n);
		return;
	}

	FLAG_TYPE tokenized = flag_tokenize(tokens[0]);

	switch (tokenized)
	{
	case (FLAG_TYPE::NONEXISTENT):
	{
		err("Unrecognized flag directive type", line_n);
		break;
	}

	case (FLAG_TYPE::OUTPUT_FILE_EXTENSION):
	{
		if (tokens.size() < OUTPUT_FILE_EXTENSION_ARGS)
		{
			err("Incorrect arguments to output file extension flag directive", line_n);
			return;
		}

		m_flags[FLAG_TYPE::OUTPUT_FILE_EXTENSION] = tokens[1];
		break;
	}
	}
}

const std::uint8_t MIN_ALIAS_ARGS = 2;

void compiler_t::func_define(std::vector<std::string> tokens, std::uint64_t line_n)
{
	if (tokens.size() < MIN_ALIAS_ARGS)
	{
		err("Incorrect arguments to define directive", line_n);
		return;
	}

	std::string key = tokens[0];
	tokens.erase(tokens.begin());

	std::string value = vector_collect(tokens, " ");

	// Don't want newlines screwing everything up like they always do
	string_replace(value, "\n", "");
	string_replace(value, "\r", "");

	alias_t alias = alias_t(key, value);
	m_aliases.push_back(alias);
}

auto is_alias_k_greator(const alias_t &a, const alias_t &b) -> bool { return (a.key.length() > b.key.length()); }

void compiler_t::sort_aliases_length() { std::sort(m_aliases.begin(), m_aliases.end(), is_alias_k_greator); }

void compiler_t::fill_aliases(std::string &res)
{
	if (m_aliases.size() == 0)
		return;

	sort_aliases_length(); // Don't want aliases replacing parts of other aliases

	for (auto alias : m_aliases)
	{
		string_replace(res, LEXICON::DEREF + alias.key, alias.value);
	}
}

void compiler_t::build_project(std::string parent_dir)
{
	auto start = std::chrono::high_resolution_clock::now();

	if (std::filesystem::exists(parent_dir) == false)
	{
		err("Project does not exist!");
		return;
	}

	std::filesystem::current_path(parent_dir);

	std::cout << "[info] Operating in: " << std::filesystem::current_path() << std::endl << std::endl;
	load_config();

	if (m_running == false)
		return;

	if (std::filesystem::exists(DIR::SOURCE) == false)
	{
		std::stringstream msg;
		msg << "Unable to locate source " << "\"" << DIR::SOURCE << "\"" << " directory.";

		err(msg.str());
		return;
	}

	if (std::filesystem::exists(DIR::BUILD) == true)
	{
		std::filesystem::remove_all(DIR::BUILD);
	}

	std::filesystem::copy(DIR::SOURCE, "./build", std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);

	compile_project();

	if (m_running == false)
		return;

	clean_project();

	auto end = std::chrono::high_resolution_clock::now();
	auto s = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << std::endl;

	std::cout << "[info] Build completed in " << MILLI_TO_U * s.count() << " seconds." << std::endl;
}

void compiler_t::compile_project()
{
	for (const auto &dir_entry : std::filesystem::recursive_directory_iterator(DIR::SOURCE))
	{
		std::stringstream stream_name;
		stream_name << dir_entry;
		std::string name = stream_name.str();

		// Ignore non-temppura files
		if (in_str(name, FILE_NAME::EXTENSION) == false)
		{
			continue;
		}

		std::string trimmed_name = trim_first_and_last(name);

		std::cout << "[info] Building file " << trimmed_name << std::endl;

		std::vector<std::string> compile_path_tokens = string_split(trimmed_name, ddelim());
		compile_path_tokens.erase(compile_path_tokens.end());
		compile_path_tokens.erase(compile_path_tokens.begin());
		compile_path_tokens.insert(compile_path_tokens.begin(), DIR::BUILD);

		std::string filename = last_in_path(trimmed_name);

		if (std::filesystem::exists(trimmed_name) == false)
			continue;

		std::string compiled = compile(file_read(trimmed_name).value());

		// Output file extension
		std::string output_fextension = m_config.default_fextension;
		if (has_flag(FLAG_TYPE::OUTPUT_FILE_EXTENSION) == true)
		{
			output_fextension = m_flags.at(FLAG_TYPE::OUTPUT_FILE_EXTENSION);
		}

		std::string renamed_filename = trim_suffix(filename, FILE_NAME::EXTENSION);
		renamed_filename += output_fextension;

		std::string compile_path = vector_collect(compile_path_tokens, ddelim());

		std::stringstream full_path;
		full_path << compile_path;
		full_path << ddelim();
		full_path << renamed_filename;

		if (m_running == false)
		{
			break;
		}

		std::cout << "[info] Outputting as " << full_path.str() << std::endl;

		file_write(full_path.str(), compiled);

		m_flags.clear();
	}
}

void compiler_t::clean_project()
{
	std::cout << "[info] Cleaning project..." << std::endl;

	for (const auto &dir_entry : std::filesystem::recursive_directory_iterator(DIR::BUILD))
	{
		std::stringstream stream_name;
		stream_name << dir_entry;
		std::string name = stream_name.str();

		if (name.find(FILE_NAME::EXTENSION) == std::string::npos)
		{
			continue;
		}

		std::filesystem::remove(dir_entry);
	}
}