#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>

#include "Utilities/Autocompletion/autocompletion.h"
#include "Utilities/Commands/commands.h"

using namespace std;
namespace fs = filesystem;

vector<string> cmdHistory;

// REPL logic
void repl() {
  // stores the input
  //string input;

  // REPL
  while (true) {
    // read the prompt
    char* input = readline("$ ");

    // EOF / Ctrl-D
    if (!input) {
      break;
    }

    string inp = string(input);
    //addToHistory(inp);                // adds to history.txt

    // don’t store empty lines
    if (*input) {
      add_history(input);             // adds to the memory using readline library
    }

    // the exit bulletin
    if (inp == "exit 0") {
      return;
      free(input);
    }

    // process the standard input and run commands, then free the input
    run(inp);
    free(input);
  }
}

int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;

  rl_initialize();
  rl_bind_key('\t', rl_complete);
  rl_attempted_completion_function = completer;   //  👈 custom completer

  using_history();                     // enable ↑ / ↓ history

  // safely get HISTFILE and read history
  const char* hf = getenv("HISTFILE");
  if (hf != nullptr && hf[0] != '\0') {
    read_history(hf);
  }

  // start the shell
  repl();

  // write the session history to HISTFILE
  write_history(hf);
}