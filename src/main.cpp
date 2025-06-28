#include <iostream>
#include <string>

using namespace std;

string extractCommand(const string& input) {
  int start = 0;
  int end = input.find(' ');
  int sizeCmd = end - start;
  string command = input.substr(start, sizeCmd);

  return command;
}

void echo(string& input) {
  int start = input.find(' ') + 1;
  int end = input.length();
  int sizeOfEchoStr = end - start;

  string echoStr = input.substr(start, sizeOfEchoStr);

  cout << echoStr << endl;
}

void type(string& input) {
  int start = input.find(' ') + 1;
  int end = input.length();
  int sizeOfTypeStr = end - start;

  string typeStr = input.substr(start, sizeOfTypeStr);
  string outputText = "";

  enum Types {echo, exit, type, invalid};
  Types Type = invalid;

  if (typeStr == "echo") Type = echo;
  else if (typeStr == "exit") Type = exit;
  else if (typeStr == "type") Type = type;

  switch (Type) {
    case echo:
      outputText = "echo is a shell builtin";
      break;
    case exit:
      outputText = "exit is a shell builtin";
      break;
    case type:
      outputText = "type is a shell builtin";
      break;
    default:
      outputText = typeStr + ": not found";
      break;
  }

  cout << outputText << endl;
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
