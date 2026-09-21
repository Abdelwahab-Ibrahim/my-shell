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
    vector<Token> tokens = tokenize(command);
    if (tokens.empty() || tokens[0].type != TokenType::WORD)
      continue;

    execute_cmd(tokens, running);
  }
}
