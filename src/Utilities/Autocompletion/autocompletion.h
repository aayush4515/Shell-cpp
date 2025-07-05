#ifndef _AUTOCOMPLETION_
#define _AUTOCOMPLETION_

#include <cstdlib>
#include <cstdio>
#include <readline/readline.h>
#include <readline/history.h>
#include <string>

char* command_generator(const char* text, int state);
char** completer(const char* text, int start, int end);

#endif