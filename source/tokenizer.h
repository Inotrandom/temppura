#ifndef H_TOKENIZER
#define H_TOKENIZER

#include <map>
#include <string>

#include "lexicon.h"

enum TOKEN
{
    NONEXISTENT,
    DEFINE
};

const std::map<std::string, TOKEN> tokenizer =
{
    {"define", TOKEN::DEFINE},
};

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

#endif