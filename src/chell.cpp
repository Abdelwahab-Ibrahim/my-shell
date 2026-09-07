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


#include "executor.h"
using namespace std;

#define GREEN   "\033[1;32m"
#define RESET   "\033[0m"

#define PRIMARY "\033[1;36m"
#define SUCCESS "\033[1;32m"
#define WARNING "\033[1;33m"
#define ERROR   "\033[1;31m"
#define INFO    "\033[1;34m"
#define MUTED   "\033[0;37m"

// Check if a command is a builtin
bool isBuiltin(const string& cmd)
{
    return cmd == "exit"
        || cmd == "echo"
        || cmd == "type"
        || cmd == "pwd";
}


// Split command into arguments
vector<string> split(const string& str, char delimiter = ' ')
{
    vector<string> result;
    stringstream ss(str);
    string part;

    while (getline(ss, part, delimiter))
    {
        if (!part.empty())
            result.push_back(part);
    }

    return result;
}


// Check if external command exists in PATH
string checkCMD(const string& cmd)
{
    const char* path_env = getenv("PATH");

    if (!path_env)
        return "";

    stringstream ss_path(path_env);
    string path;

    while (getline(ss_path, path, ':'))
    {
        string full_path = path + '/' + cmd;

        if (access(full_path.c_str(), X_OK) == 0)
            return full_path;
    }

    return "";
}


// Parse command into command + arguments
pair<string, string> parseCommand(const string& input)
{
    stringstream ss(input);

    string cmd;
    string args;

    ss >> cmd;
    getline(ss, args);

    // Remove leading space
    if (!args.empty() && args[0] == ' ')
        args.erase(0, 1);

    return {cmd, args};
}


// Built-in: exit
void exitCommand(bool& running)
{
    running = false;
}


// Built-in: echo
void echoCommand(const string& args)
{
    cout << GREEN << args << RESET << endl;
}

// Built-in: pwd
void pwdCommand()
{
    cout << GREEN << filesystem::current_path().string() << RESET << endl;
}

// Built-in: type
void typeCommand(const string& cmd)
{
    if (isBuiltin(cmd))
    {
        cout << GREEN << cmd << RESET
             << ": is a " << GREEN << "builtin" << RESET << endl;

        return;
    }

    string full_path = checkCMD(cmd);

    if (!full_path.empty())
    {
        cout << GREEN << cmd << RESET
             << " is " << GREEN << full_path << RESET << endl;

        return;
    }

    cout << ERROR << cmd << RESET
         << ": command not found" << endl;
}








// Execute builtin command
void executeBuiltin(const string& cmd, const string& args, bool& running)
{
    if (cmd == "exit")
    {
        exitCommand(running);
    }
    else if (cmd == "echo")
    {
        echoCommand(args);
    }
    else if (cmd == "type")
    {
        typeCommand(args);
    }
    else if (cmd == "pwd")
    {
        pwdCommand();
    }
}


int main()
{
    cout << unitbuf;
    cerr << unitbuf;

    bool running = true;

    while (running)
    {
        cout << WARNING << "$ " << MUTED;

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
