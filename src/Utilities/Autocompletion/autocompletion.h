#ifndef _AUTOCOMPLETION_
#define _AUTOCOMPLETION_
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <readline/readline.h>
#include <readline/history.h>
#include <string>
#include <cstring>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <filesystem>
#include <unistd.h>
#include "../Helpers/helpers.h"

using namespace std;
namespace fs = filesystem;

char* command_generator(const char* text, int state);               // generates commands for autocompletion
char** completer(const char* text, int start, int end);             // autocompletes the commands

#endif