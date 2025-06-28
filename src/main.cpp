#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

// extracts the path and returns splitted directories in a vector
void extractPath () {
  // stores the directories from the path variables
  vector<string> pathDirectories;

  // extracting the path variable
  const char* envVariable = "PATH";
  string path = string(getenv(envVariable));

  // splitting the path variables into directories and storing in the vector pathDirectories

  // thought process:

  // start a while loop (or maybe a for-loop)
  // keep looking for coloms (:) to separate the directories
  // as soon as the first one is found, push everything until that index - 1 to the vector
  // delete everything until that colon, including the colon

  for (int i = 0; i < path.length(); i++) {
    string currDirectory = "";

    if (path.at(i) == ':') {
      currDirectory = path.substr(0, i + 1);
      path.erase(0, i + 1);
    }
    pathDirectories.push_back(currDirectory);
  }

  for (const auto& dir : pathDirectories) {
    cout << dir << endl;
  }
}

// void searchPath (vector<string> pathDirectories) {

//   // FIXME: searches every single directory in the path for the given commans
//   // FIXME: might require a second parameter to accept the command being sought for
// }


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

  string typeArg = input.substr(start, sizeOfTypeStr);
  string outputText = "";

  enum Types {echo, exit, type, invalid};
  Types Type = invalid;

  if (typeArg == "echo") Type = echo;
  else if (typeArg == "exit") Type = exit;
  else if (typeArg == "type") Type = type;

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
      outputText = typeArg + ": not found";
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
  //repl(input);

  //searchPath();
  extractPath();

}
