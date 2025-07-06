#ifndef _HELPERS_
#define _HELPERS_

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>


using namespace std;

bool hasPipe(string& input);                                    // determines if input has pipe(s)
bool isValidCommand(string cmd);                                // determines if a command is valid
bool isBuiltin(const string& cmd);                              // determines if a command is built-in
bool isExternalExecutableCommand(const string& cmd);            // determines is the command is external (not built-in) and executable

void runBuiltin(string& cmd, string& input);                    // runs a built-in command
void run(string& input);                                        // processes and runs the input string, wraps runBuiltin() function
void addToHistory(string& input);                               // adds command history to external file
void displayHistory();                                          // displays in-built history to stdout
void displayHistory(int n);                                     // displays in-built history to stdout -> last 'n' lines
void searchPath (string targetFile);                            // searches the $PATH varuable for a targetFile

string extractCommand(const string& input);                     // extracts the command from input string

vector<string> extractExternalCommands();                       // extracts external commands from $PATH
vector<string> splitOnPipe(const string& input);                // splits the input on pipes
vector<string> extractPath();                                   // extracts paths from $PATH variable

#endif