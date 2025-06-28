#include <iostream>
#include <string>

using namespace std;

void echo(string& input) {
  int start = input.find(' ') + 1;
  int end = input.length();
  int sizeOfEchoStr = end - start;

  string echoStr = input.substr(start, sizeOfEchoStr);

  cout << echoStr << endl;
}

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

      // check for echo
      if (input.find("echo") != string::npos) {
        echo(input);
        continue;
      }

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

  // stores the input command
  string input;

  // start the shell
  repl(input);

}
