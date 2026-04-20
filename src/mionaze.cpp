#include <iostream>
#include <string>
#include <sstream>   


using namespace std;
int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // TODO: Uncomment the code below to pass the first stage
  // the loop
  while (true)
  { 
    std::cout << "$ ";
    string command;
    std::getline(std::cin, command);
    if(command == "exit") break;
    
    stringstream ss(command);
    string cmd,argument;
    ss>> cmd;
    getline(ss,argument);
    if(cmd == "echo") cout<< argument<<endl;
    else cout << cmd << ": command not found"<< endl;
  }
}
