#ifndef BUILTINS_HPP
#define BUILTINS_HPP

#include "lexer.hpp"

#include <string>
#include <vector>
using namespace std;

void execute_cmd(const vector<Token> &tokens, bool &running);

bool isBuiltin(const string &cmd);

#endif
