#include <iostream>
#include <string>
#include <sstream>

using namespace std;

#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"

int main() {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    while (true)
    {
        cout << "$ ";

        string command;
        getline(cin, command);

        if (command == "exit")
            break;

        stringstream ss(command);
        string cmd, argument;

        ss >> cmd;
        getline(ss, argument);

        // remove leading space from argument (important)
        if (!argument.empty() && argument[0] == ' ')
            argument.erase(0, 1);

        if (cmd == "echo")
        {
            cout << GREEN << argument << RESET << endl;
        }
        else if (!cmd.empty())
        {
            cout << RED << cmd << RESET << ": command not found" << endl;
        }
    }

    return 0;
}