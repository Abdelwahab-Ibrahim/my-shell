#ifndef REDIRECTION_HPP
#define REDIRECTION_HPP

#include <vector>
#include "lexer.hpp"

bool applyRedirections(
    const std::vector<Token> &tokens,
    int &saved_stdout,
    int &saved_stderr);

void restoreRedirections(
    int saved_stdout,
    int saved_stderr);

#endif