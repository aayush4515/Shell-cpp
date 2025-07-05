#include "autocompletion.h"
using namespace std;

char* command_generator(const char* text, int state)
{
    static const char* const cmds[] = {"cd", "ls", "exit", "echo", "type", "mkdir", "help", nullptr};
    static int index, len;

    if (!state) {                        // first call
        index = 0;
        len   = strlen(text);
    }

    while (cmds[index]) {
        const char* name = cmds[index++];
        if (strncmp(name, text, len) == 0)
            return strdup(name);         // readline frees this for you
    }
    return nullptr;
}

char** completer(const char* text, int start, int end)
{
    /* If we are at the start of the line, complete shell commands.
       Otherwise fall back to default filename completion.              */
    if (start == 0)
        return rl_completion_matches(text, command_generator);

    return nullptr;   // use default
}