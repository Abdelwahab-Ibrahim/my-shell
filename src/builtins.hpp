#ifndef BUILTINS_H
#define BUILTINS_H

#include <string>
using namespace std;

void executeBuiltin(const string &cmd, const string &args, bool &running);

bool isBuiltin(const string &cmd);

#endif
