#pragma once

#include <vector>
#include <string>
using namespace std;
enum class State
{
    Plain,
    InSingleQuote,
    InDoubleQuote,
    EscapedOut,
    EscapedInDoubleQuote,
    Redirect,
    Collect
};

enum class TokenType
{
    WORD,
    STD_OUT,
    STD_OUT_APPEND,
    STD_ERR,
    STD_ERR_APPEND,
    PIPE,
    EOL
};

struct Token
{
    TokenType type;
    string value;
};

std::vector<Token> tokenize(const std::string &cmd);
