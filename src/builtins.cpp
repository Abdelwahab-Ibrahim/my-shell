#include "builtins.hpp"
#include "utils.hpp"
#include "style.cpp"
#include "lexer.hpp"
#include <fstream>
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
             << " is a " << GREEN << "shell builtin" << RESET << endl;

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
         << ": not found" << endl;
}

// Built-in: exit
void exitCommand(bool &running)
{
    running = false;
}

// Built-in: echo
void echoCommand(const string &args)
{
    vector<Token> tokens = tokenize(args);
    string content = "";

    // fstream file;
    for (auto it = tokens.begin(); it != tokens.end(); it++)
    {
        if (it->type == TokenType::WORD)
        {
            content += it->value;
            if(it != (tokens.end()-1))
                content += ' ';
        }
        // else if (it->type == TokenType::STD_OUT)
        // {
        //     string filename = it->value;
        //     if (filename.empty())
        //     {
        //         cout << ERROR << "ERROR: " << RESET
        //              << "Expected file name after '>'" << endl;
        //         return;
        //     }
        //     else
        //     {

        //         file.open(filename, ios::out);

        //     }
        // }
    }

    // if(file.is_open()){
    //     file << content;
    //     file.close();
    // }else{
        cout << GREEN << content << endl;
    // }
    
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
        }
        else
        {

            filesystem::path target = filesystem::absolute(path);
            filesystem::current_path(target);
        }
    }
    catch (const filesystem::filesystem_error &e)
    {
        cout << ERROR << "cd: " << path << ": No such file or directory" << RESET << endl;
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
