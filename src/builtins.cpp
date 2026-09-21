#include "builtins.hpp"
#include "utils.hpp"
#include "lexer.hpp"
#include "redirection.hpp"
#include "style.cpp"
#include "executor.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <filesystem>

using namespace std;

// Built-in: type
void typeCommand(const vector<Token> &tokens)
{
    for (size_t i = 1; i < tokens.size(); i++)
    {
        if (tokens[i].type != TokenType::WORD)
            continue;

        const string &cmd = tokens[i].value;
        if (isBuiltin(cmd))
        {
            cout << GREEN << cmd << RESET
                 << " is a " << GREEN << "shell builtin" << RESET << endl;

            continue;
        }

        string full_path = checkCMD(cmd);

        if (!full_path.empty())
        {
            cout << GREEN << cmd << RESET
                 << " is " << GREEN << full_path << RESET << endl;

            continue;
        }

        cout << ERROR << cmd << RESET
             << ": not found" << endl;
    }
}

// Built-in: exit
void exitCommand(bool &running)
{
    running = false;
}

// Built-in: echo
void echoCommand(const vector<Token> &tokens)
{
    string content = "";

    bool first = true;
    for (size_t i = 1; i < tokens.size(); i++)
    {
        if (tokens[i].type == TokenType::WORD)
        {
            if (!first)
                content += ' ';
            content += tokens[i].value;
            first = false;
        }
    }

    cout << content << endl;
}

// Built-in: pwd
void pwdCommand()
{
    cout << GREEN << filesystem::current_path().string() << RESET << endl;
}

// Built-in: cd
void cdCommand(const vector<Token> &tokens)
{
    string path;
    for (size_t i = 1; i < tokens.size(); i++)
    {
        if (tokens[i].type == TokenType::WORD)
        {
            path = tokens[i].value;
            break;
        }
    }

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
void execute_cmd(const vector<Token> &tokens, bool &running)
{
    int saved_stdout;
    int saved_stderr;
    if (tokens.empty() || tokens[0].type != TokenType::WORD)
        return;

    string cmd = tokens[0].value;
    if (!applyRedirections(tokens, saved_stdout, saved_stderr))
        return;

    if (cmd == "exit")
    {
        exitCommand(running);
    }
    else if (cmd == "echo")
    {
        echoCommand(tokens);
    }
    else if (cmd == "type")
    {
        typeCommand(tokens);
    }
    else if (cmd == "pwd")
    {
        pwdCommand();
    }
    else if (cmd == "cd")
    {
        cdCommand(tokens);
    }
    else
    {
        run_external(tokens);
    }

    restoreRedirections(saved_stdout, saved_stderr);
}
