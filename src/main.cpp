#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <functional>
#include <unistd.h>
#include <sstream>
#include "Utilities/Commands/commands.h"

using namespace std;
namespace fs = filesystem;

void repl(string& input) {
    // REPL
    while (true) {
      // prompt
      cout << "$ ";

      // read the prompt
      getline(cin, input);

      // the exit bulletin
      if (input == "exit 0") {
        return;
      }

      // extract the command
      string command = extractCommand(input);

      // check for various commands
      if (command == "echo") {
        echo(input);
        continue;
      }
      else if (command == "type") {
        type(input);
        continue;
      }
      else if (command == "pwd") {
        pwd();
        continue;
      }
      else if (command == "cd") {
        cd(input);
        continue;
      }
      else if (isExternalExecutableCommand(command)) {
        system(input.c_str());
      }
      // output as invalid command
      else {
        cout << input << ": command not found" << endl;
      }

      // ready for next command
      input = "";
    }
}


int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;

  // stores the input command
  string input;

  // start the shell
  repl(input);

}
