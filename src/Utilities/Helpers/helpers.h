#ifndef _HELPERS_
#define _HELPERS_

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>


using namespace std;

vector<string> extractPath ();
void searchPath (string targetFile);
bool isExternalExecutableCommand(const string& cmd);
string extractCommand(const string& input);
vector<string> extractExternalCommands();
bool isValidCommand(string cmd);
bool isBuiltin(const string& cmd);
void runBuiltin(string& cmd, string& input);
void run(string& input);

#endif