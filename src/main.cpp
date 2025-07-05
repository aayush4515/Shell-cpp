#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "Utilities/Autocompletion/autocompletion.h"
#include "Utilities/Commands/commands.h"

using namespace std;
namespace fs = filesystem;

// REPL logic
void repl() {
  // stores the input
  //string input;

  // REPL
  while (true) {
    //   cout << "$ ";

    // // read the prompt
    // getline(cin, input);

    char* input = readline("$ ");

    // EOF / Ctrl-D
    if (!input) {
      break;
    }

     // don’t store empty lines
    if (*input) {
      add_history(input);
    }

    string inp = string(input);

    if (hasPipe(inp)) {
      vector<string> stages = splitOnPipe(inp);
      for (const auto& stage : stages) {
        cout << stage << endl;
      }
    }

    continue;

    // the exit bulletin
    if (inp == "exit 0") {
      return;
    }

    // process the standard input and run commands
    run(inp);

    // ready for next command
    //input = "";

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

  // start the shell
  repl();

}