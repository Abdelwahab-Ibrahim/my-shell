#include "executor.hpp"
#include "lexer.hpp"

#include <iostream>
#include <unistd.h>
#include <csignal>
#include <cstdlib>
#include <sys/wait.h>

void run_external(const std::string &path, std::string &args)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        // Child process
        signal(SIGINT, SIG_DFL);

        std::vector<char *> argv;

        vector<Token> tokens = tokenize(args);

        for (auto it = tokens.begin(); it != tokens.end(); it++)
        {
            string arg = it->value;
            if (it->type == TokenType::WORD)
            {
                argv.push_back(const_cast<char *>(arg.c_str()));
            }
        }

        argv.push_back(nullptr);

        execv(path.c_str(), argv.data());

        perror("execv");
        exit(EXIT_FAILURE);
    }

    // Parent process
    waitpid(pid, nullptr, 0);
}
