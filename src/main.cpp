#include <iostream>
#include <string>

#include "builtins.hpp"
#include "utils.hpp"
#include "style.cpp" 
#include "executor.hpp"

using namespace std;
int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // TODO: Uncomment the code below to pass the first stage
  // the loop
  bool running = true;
  while (running)
  {
    std::cout << "$ ";
    string command;
    std::getline(std::cin, command);
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
        cout << command << ": command not found" << endl;
    }
  }
}
