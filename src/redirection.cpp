#include "redirection.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <iostream>

using namespace std;

bool applyRedirections(
    const vector<Token> &tokens,
    int &saved_stdout,
    int &saved_stderr)
{
    saved_stdout = dup(STDOUT_FILENO);
    saved_stderr = dup(STDERR_FILENO);

    if (saved_stdout == -1 || saved_stderr == -1)
    {
        perror("dup");
        return false;
    }

    for (const auto &token : tokens)
    {
        int fd;
        int flags;

        if (token.type == TokenType::STD_OUT)
        {
            fd = STDOUT_FILENO;
            flags = O_WRONLY | O_CREAT | O_TRUNC;
        }
        else if (token.type == TokenType::STD_OUT_APPEND)
        {
            fd = STDOUT_FILENO;
            flags = O_WRONLY | O_CREAT | O_APPEND;
        }
        else if (token.type == TokenType::STD_ERR)
        {
            fd = STDERR_FILENO;
            flags = O_WRONLY | O_CREAT | O_TRUNC;
        }
        else if (token.type == TokenType::STD_ERR_APPEND)
        {
            fd = STDERR_FILENO;
            flags = O_WRONLY | O_CREAT | O_APPEND;
        }
        else
        {
            continue;
        }

        int file = open(token.value.c_str(), flags, 0644);

        if (file == -1)
        {
            perror(token.value.c_str());
            return false;
        }

        if (dup2(file, fd) == -1)
        {
            perror("dup2");
            close(file);
            return false;
        }

        close(file);
    }

    return true;
}

void restoreRedirections(
    int saved_stdout,
    int saved_stderr)
{
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stderr, STDERR_FILENO);

    close(saved_stdout);
    close(saved_stderr);
}