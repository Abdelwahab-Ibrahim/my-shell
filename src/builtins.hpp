#ifndef BUILTINS_HPP
#define BUILTINS_HPP

#include <string>
using namespace std;

void executeBuiltin(const string &cmd, const string &args, bool &running);

bool isBuiltin(const string &cmd);

#endif
