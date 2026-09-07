#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <unistd.h>
#include <cstdlib>
#include <csignal>
#include <filesystem>
#include <sys/wait.h>
#include <sys/types.h>


#include "src/executor.h"
#include "src/builtins.h"
#include "src/utils.h"
#include "src/style.cpp"
using namespace std;

string getCurrentDirectory()
{
    // replace home with ~
    string home = getenv("HOME");
    if (home.empty()){
        return filesystem::current_path().string();
    }
    // Replace home with ~
    string current_dir = filesystem::current_path().string();
    if (current_dir.substr(0, home.length()) == home) {
        current_dir = "~" + current_dir.substr(home.length());
    }
    return current_dir;
}

int main()
{
    cout << unitbuf;
    cerr << unitbuf;

    bool running = true;

    while (running)
    {
        cout << PRIMARY << getCurrentDirectory() << WARNING << " $ " << MUTED;

        string command;
        getline(cin, command);

        auto [cmd, args] = parseCommand(command);

        if (cmd.empty())
            continue;

        if (isBuiltin(cmd))
        {
            executeBuiltin(cmd, args, running);
        }
        else
        {
            string path = checkCMD(cmd);

            if (!path.empty())
            {
                vector<string> argv = split(command);
                run_external(path, argv);
            }
            else
            {
                cout << ERROR << cmd << RESET
                     << ": command not found" << endl;
            }
        }
    }

    return 0;
}
