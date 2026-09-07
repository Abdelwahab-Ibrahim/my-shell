#include "utils.h"
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
using namespace std;

pair<string, string> parseCommand(const string &input)
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

// Split command into arguments
vector<string> split(const string &str, char delimiter)
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
string checkCMD(const string &cmd)
{
    const char *path_env = getenv("PATH");

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