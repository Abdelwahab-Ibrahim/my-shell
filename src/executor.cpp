#include "executor.h"

#include <iostream>
#include <unistd.h>
#include <csignal>
#include <cstdlib>
#include <sys/wait.h>

void run_external(const std::string& path, const std::vector<std::string>& args)
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

        std::vector<char*> argv;

        for (const std::string& arg : args)
        {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }

        argv.push_back(nullptr);

        execv(path.c_str(), argv.data());

        perror("execv");
        exit(EXIT_FAILURE);
    }

    // Parent process
    waitpid(pid, nullptr, 0);
}
