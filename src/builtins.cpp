#include "builtins.h"
#include "utils.h"
#include "style.cpp"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <filesystem>

using namespace std;

// Built-in: type
void typeCommand(const string &cmd)
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

// Built-in: exit
void exitCommand(bool &running)
{
    running = false;
}

// Built-in: echo
void echoCommand(const string &args)
{
    cout << GREEN << args << RESET << endl;
}

// Built-in: pwd
void pwdCommand()
{
    cout << GREEN << filesystem::current_path().string() << RESET << endl;
}

// Built-in: cd
void cdCommand(const string &path = "")
{

    try
    {
        if (path.empty() || path == "~")
        {
            filesystem::current_path(filesystem::path(getenv("HOME")));
        }
        else if (path.rfind("~/", 0) == 0)
        {
            filesystem::path target = filesystem::path(getenv("HOME")) / path.substr(2);
            filesystem::current_path(target);
        }else {

            
            filesystem::path target = filesystem::absolute(path);
            filesystem::current_path(target);
        }
    }
    catch (const filesystem::filesystem_error &e)
    {
        cout << ERROR << e.what() << RESET << endl;
    }
}

// Check if a command is a builtin
bool isBuiltin(const string &cmd)
{
    return cmd == "exit" || cmd == "echo" || cmd == "type" || cmd == "pwd" || cmd == "cd";
}

// Execute builtin command
void executeBuiltin(const string &cmd, const string &args, bool &running)
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
    else if (cmd == "cd")
    {
        cdCommand(args);
    }
}
