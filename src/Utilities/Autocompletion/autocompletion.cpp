#include "autocompletion.h"
using namespace std;

char* command_generator(const char* text, int state)
{
    // some built-in commands
    vector<string> commands = {"cd", "ls", "exit", "echo", "type", "mkdir", "help"};

    // append external commands to commands vector
    vector<string> externalCommands = extractExternalCommands();
    for (const auto& cmd : externalCommands) {
        commands.push_back(cmd);
    }

    size_t index, len;

    if (!state) {                        // first call
        index = 0;
        len   = strlen(text);
    }

    while (index < commands.size()) {
        const string& name = commands[index++];
        /* compare the first `len` characters with the user’s prefix */
        if (name.compare(0, len, text, len) == 0) {
            return strdup(name.c_str());   // Readline will free it
        }
    }
    return nullptr;
}

char** completer(const char* text, int start, int end)
{
    // took an hour and half to figure this out, documentation helped!
    rl_delete_text(start, end);
    rl_replace_line("", 0);
    /* If we are at the start of the line, complete shell commands.
       Otherwise fall back to default filename completion.              */
    if (start == 0) {
        return rl_completion_matches(text, command_generator);
    }

    return nullptr;   // use default
}
