#include "executor.hpp"
#include "builtins.hpp"
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

void run_external(const vector<Token> &tokens)
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

void runPipeline(const vector<vector<Token>> &commands)
{
    if (commands.empty())
        return;

    vector<pid_t> children;
    int previous_read = -1;

    for (size_t i = 0; i < commands.size(); i++)
    {
        int pipe_fds[2] = {-1, -1};
        if (i + 1 < commands.size() && pipe(pipe_fds) == -1)
        {
            perror("pipe");
            if (previous_read != -1)
                close(previous_read);
            return;
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            if (previous_read != -1)
                close(previous_read);
            if (pipe_fds[0] != -1)
                close(pipe_fds[0]);
            if (pipe_fds[1] != -1)
                close(pipe_fds[1]);
            return;
        }

        if (pid == 0)
        {
            signal(SIGINT, SIG_DFL);

            if (previous_read != -1)
            {
                dup2(previous_read, STDIN_FILENO);
                close(previous_read);
            }

            if (pipe_fds[1] != -1)
            {
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[1]);
            }

            if (pipe_fds[0] != -1)
                close(pipe_fds[0]);

            if (!commands[i].empty() &&
                commands[i][0].type == TokenType::WORD &&
                isBuiltin(commands[i][0].value))
            {
                bool running = true;
                execute_cmd(commands[i], running);
                _exit(0);
            }

            int saved_stdout;
            int saved_stderr;
            if (!applyRedirections(commands[i], saved_stdout, saved_stderr))
                _exit(1);

            vector<char *> argv;
            for (const Token &token : commands[i])
            {
                if (token.type == TokenType::WORD)
                    argv.push_back(const_cast<char *>(token.value.c_str()));
            }
            argv.push_back(nullptr);

            if (argv[0] == nullptr)
                _exit(0);

            string path = checkCMD(argv[0]);
            if (path.empty())
            {
                cout << argv[0] << ERROR << ": command not found" << RESET << endl;
                _exit(127);
            }

            execv(path.c_str(), argv.data());
            perror("execv");
            _exit(127);
        }

        children.push_back(pid);
        if (previous_read != -1)
            close(previous_read);
        if (pipe_fds[1] != -1)
            close(pipe_fds[1]);
        previous_read = pipe_fds[0];
    }

    if (previous_read != -1)
        close(previous_read);

    for (pid_t child : children)
        waitpid(child, nullptr, 0);
}
