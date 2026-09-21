#include <iostream>
#include <string>

#include "builtins.hpp"
#include "utils.hpp"
#include "style.cpp"
#include "executor.hpp"
#include "lexer.hpp"

using namespace std;
int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  bool running = true;
  while (running)
  {
    std::cout << "$ ";
    string command;
    std::getline(std::cin, command);
    vector<Token> commands = tokenize(command);
    if (commands.empty() || commands[0].type != TokenType::WORD)
      continue;

    if (hasPipe(commands))
    {
      vector<vector<Token>> commandGroups;
      vector<Token> currentCommand;

      for (const Token &token : commands)
      {
        if (token.type == TokenType::PIPE)
        {
          if (!currentCommand.empty())
          {
            commandGroups.push_back(currentCommand);
            currentCommand.clear();
          }
        }
        else
        {
          currentCommand.push_back(token);
        }
      }

      if (!currentCommand.empty())
      {
        commandGroups.push_back(currentCommand);
      }

      runPipeline(commandGroups);
    }
    else
    {
      execute_cmd(commands, running);
    }
  }
}
