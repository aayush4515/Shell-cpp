#include <cstdlib>
#include <cstdio>
#include <readline/readline.h>
#include <readline/history.h>
#include <string>

int main()
{
    rl_initialize();
    rl_bind_key('\t', rl_complete);
    rl_attempted_completion_function = completer;   //  👈 custom completer

    using_history();                     // enable ↑ / ↓ history

    while (true) {
        char* line = readline("prompt> ");
        if (!line) break;                // EOF / Ctrl-D

        if (*line) add_history(line);    // don’t store empty lines

        /* … do whatever you need with the input here … */

        free(line);                      // readline mallocs the buffer
    }
    return 0;
}
