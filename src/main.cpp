#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <functional>

using namespace std;
namespace fs = filesystem;

// extracts the path and returns splitted directories in a vector
vector<string> extractPath () {
  // stores the directories from the path variables
  vector<string> paths;

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
      currDirectory = path.substr(0, i);
      path.erase(0, i + 1);
      paths.push_back(currDirectory);
      i = 0;  // resets the index back to 0
    }
  }
  return paths;
}

void searchPath (string targetFile) {

  // FIXME: searches every single directory in the path for the given commands
  // FIXME: might require a second parameter to accept the command being sought for

  vector<string> paths = extractPath();

  for (const auto& path : paths) {
    try {
      for (const auto& entry : fs::directory_iterator(path)) {
          // File found
          if (entry.is_regular_file() && entry.path().filename() == targetFile) {
              // Get file permissions
              fs::perms p = fs::status(entry.path()).permissions();

              // Check for executable permissions for owner, group, or others
              if ((p & fs::perms::owner_exec) != fs::perms::none ||
                  (p & fs::perms::group_exec) != fs::perms::none ||
                  (p & fs::perms::others_exec) != fs::perms::none)
              {

                string displayPath = string(entry.path()).substr(1, string(entry.path()).length());

                cout << targetFile << " is " << displayPath << endl;
              } else {
                // continue searching if the file doesn't have exe permissions
                  continue;
              }
              return; // Exit after finding and checking the file

          }
      }
    }
    catch (const fs::filesystem_error& e) {
      // skip the ones which do not exist
      continue;
    }
  }
}

bool isValidCommand(string cmd) {
  vector<string> commands = {
    "cd", "ls", "echo", "exit", "pwd", "type", "mkdir", "rmdir", "touch",
    "cp", "mv", "rm", "cat", "clear", "whoami", "man", "which", "grep",
    "chmod", "chown", "find", "head", "tail", "diff", "history", "ps",
    "kill", "top", "nano", "vi", "ssh", "scp", "tar", "gzip", "ping"
  };

  if (find(commands.begin(), commands.end(), cmd) != commands.end()) {
    return true;
  }
  return false;
}

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
  //string outputText = "";

  vector<string> builtIns {"echo", "exit", "type", "pwd", "cd"};

  if (!isValidCommand(typeArg)) {
    cout << typeArg << ": not found" << endl;
  }
  else if (find(builtIns.begin(), builtIns.end(), typeArg) != builtIns.end()) {
    cout << typeArg << " is a shell builtin" << endl;
  }
  else {
    // call the searchPath() function
    searchPath(typeArg);
  }

  // enum Types {echo, exit, type, invalid};
  // Types Type = invalid;

  // if (typeArg == "echo") Type = echo;
  // else if (typeArg == "exit") Type = exit;
  // else if (typeArg == "type") Type = type;

  // switch (Type) {
  //   case echo:
  //     outputText = "echo is a shell builtin";
  //     break;
  //   case exit:
  //     outputText = "exit is a shell builtin";
  //     break;
  //   case type:
  //     outputText = "type is a shell builtin";
  //     break;
  //   default:
  //     outputText = typeArg + ": not found";
  //     break;
  // }

  //cout << outputText << endl;
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

  //searchPath();
  //extractPath();

}
