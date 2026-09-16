#include "executor.hpp"
#include "lexer.hpp"
#include "utils.hpp"
#include "style.cpp"
#include <iostream>
#include <unistd.h>
#include <csignal>
#include <cstdlib>
#include <sys/wait.h>

void run_external(const std::string &command)
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

        vector<Token> tokens = tokenize(command);

        for (auto it = tokens.begin(); it != tokens.end(); it++)
        {
            if (it->type == TokenType::WORD)
            {
                argv.push_back(const_cast<char *>(it->value.c_str()));
            }
        }

        argv.push_back(nullptr);
        for (auto &it : argv)
            cout << ERROR << it << RESET << endl;

        string path = argv[0];

        if (command.find('/') == std::string::npos)
        {
            path = checkCMD(argv[0]);

            if (path.empty()){
                cout << ERROR << "path not found " << RESET << endl;
                exit(EXIT_FAILURE);
            }
        }
        cout << ERROR <<"this is path "<< path << RESET << endl;
        execv(path.c_str(), argv.data());

        perror("execv");
        exit(EXIT_FAILURE);
    }

    // Parent process
    waitpid(pid, nullptr, 0);
}
