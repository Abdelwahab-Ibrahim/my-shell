#include <iostream>
#include <string>
#include <sstream>
#include<functional>
#include<unordered_map>

using namespace std;

#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"



// Modern foreground colors (soft but high contrast)
#define PRIMARY   "\033[1;36m"  // soft cyan (commands / shell name)
#define SUCCESS   "\033[1;32m"  // muted green (success output)
#define WARNING   "\033[1;33m"  // warm amber (warnings)
#define ERROR     "\033[1;31m"  // soft red (errors)
#define INFO      "\033[1;34m"  // calm blue (info output)
#define MUTED     "\033[0;37m"  // dim gray (secondary text)



pair<string, string> parseCommand(const string& input) {
    stringstream ss(input);
    string cmd, args;

    ss >> cmd;
    getline(ss, args);

    // remove leading space
    if (!args.empty() && args[0] == ' ')
        args.erase(0, 1);

    return {cmd, args};
}


int main() {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  
  bool running = true;


    // command registry uomap
    unordered_map<string, function<void(string&)>> commands;

    commands["exit"] = [&running](string&) {
        running = false;
    };


    commands["echo"]=[](string &Args){
      cout << GREEN << Args << RESET << endl;
    };


    commands["type"]=[&](string &CMD){
      if(commands.count(CMD))
          cout << GREEN << CMD << RESET<< ": is a "<< GREEN << "built in" << endl;
      else cout << ERROR << CMD << RESET<< ": is not a "<< ERROR << "built in" << endl;
    };


    while (running)
    {
        cout <<WARNING<< "$ "<< MUTED;

        string command;
        getline(cin, command);

        
        auto [cmd ,args] = parseCommand(command);

        if (commands.count(cmd)) commands[cmd](args);
        else cout << ERROR << cmd << RESET<<": command not found"<< endl;

        
    }

    return 0;
}