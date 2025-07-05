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
    string inp = string(input);
    addToHistory(inp);

    // EOF / Ctrl-D
    if (!input) {
      break;
    }

     // don’t store empty lines
    if (*input) {
      add_history(input);
    }



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

  // clear history.txt
  ofstream outFile("history.txt");
  outFile.close();

  // start the shell
  repl();
}