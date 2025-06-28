#include <iostream>
#include <string>

using namespace std;

void repl() {
    // stores the input command
    string input;

    // REPL
    while (true) {
      // prompt
      cout << "$ ";

      // read the prompt
      getline(cin, input);

      // output as invalid command
      cout << input << ": command not found" << endl;

      // ready for next command
      input = "";
    }
}

int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;

  // start the shell
  repl();

}
