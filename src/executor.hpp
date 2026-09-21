#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "lexer.hpp"

#include <string>
#include <vector>

void run_external(const vector<Token> &tokens);

void runPipeline(const vector<vector<Token>> &commands);

#endif
