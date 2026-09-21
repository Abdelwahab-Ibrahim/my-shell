#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
using namespace std;

pair<string, string> parseCommand(const string &input);

vector<string> split(const string &str, char delimiter = ' ');

bool hasPipe(const vector<Token> &tokens);

string checkCMD(const string &cmd);

#endif
