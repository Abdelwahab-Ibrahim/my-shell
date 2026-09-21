#include "executor.hpp"
#include "lexer.hpp"
#include "utils.hpp"
#include "redirection.hpp"
#include "style.cpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <csignal>
#include <cstdlib>
#include <sys/wait.h>

void run_external(vector<Token> &tokens)
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

        int saved_stdout;
        int saved_stderr;
        
        // for (const auto &token : tokens)
        // {
        //     cout << "TYPE: " << static_cast<int>(token.type)
        //          << " VALUE: [" << token.value << "]\n";
        // }
        if (!applyRedirections(tokens, saved_stdout, saved_stderr))
            _exit(1);
        std::vector<char *> argv;

        for (auto it = tokens.begin(); it != tokens.end(); it++)
        {
            if (it->type == TokenType::WORD)
            {
                argv.push_back(const_cast<char *>(it->value.c_str()));
            }
        }

        argv.push_back(nullptr);
        // for (auto &it : argv)
            // cout << ERROR << it << RESET << endl;

        string path = checkCMD(argv[0]);

        if (path.empty())
        {
            cout << argv[0] << ERROR << ": command not found" << RESET << endl;
            exit(EXIT_FAILURE);
        }

        execv(path.c_str(), argv.data());

        perror("execv");
        exit(EXIT_FAILURE);
    }

    // Parent process
    waitpid(pid, nullptr, 0);
}
