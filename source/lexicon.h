#ifndef H_LEXICON
#define H_LEXICON

#include <string>

namespace LEXICON
{
const std::string DELIM = "\n";
const std::string TOKEN_DELIM = " ";

const std::string PREPROC = "$BEGINCOMMENTppura::";
const std::string DEFINE = "define";
const std::string FLAG = "flag";

const std::string DEREF = "$";
} // namespace LEXICON

namespace DIR
{
const std::string SOURCE = "src";
const std::string BUILD = "build";
} // namespace DIR

namespace FILE_NAME
{
const std::string EXTENSION = ".ppura";
const std::string CONFIG = "temppura.cfg";
} // namespace FILE_NAME

#endif // H_LEXICON