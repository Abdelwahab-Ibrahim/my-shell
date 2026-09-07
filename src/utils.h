#ifndef UTILS_H
#define UTILS_H


#include <string>
#include <vector>
using namespace std;

pair<string, string> parseCommand(const string &input);
vector<string> split(const string &str, char delimiter = ' ');

string checkCMD(const string &cmd);


#endif
