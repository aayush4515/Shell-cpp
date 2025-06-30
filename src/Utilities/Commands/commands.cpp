#include "commands.h"
#include <filesystem>

namespace fs = filesystem;

void pwd() {
    string pwd = string(fs::current_path());
    cout << pwd << endl;
  }

  void cd(string& input) {
    int start = input.find(' ') + 1;
    int end = input.length();
    int sizeOfNewDirectory = end - start;

    string newDirectory = input.substr(start, sizeOfNewDirectory);

    // change to new directory
    try {
      if (newDirectory == "~") {
        newDirectory = string(getenv("HOME"));
      }
      fs::current_path(newDirectory);
    }
    catch (const fs::filesystem_error& e) {
      cerr << "cd: no such file or directory: " << newDirectory << endl;
    }

  }

  void echo(string& input) {
    int start = input.find(' ') + 1;
    int end = input.length();
    int sizeOfEchoStr = end - start;

    string echoStr = input.substr(start, sizeOfEchoStr);

    // check if the string to be echoed contains single quotes
    if (isSingleQuoted(echoStr)) {
      echoStr = handleSingleQuotes(echoStr);
    }
    // else if (isDoubleQuoted) {
    //   echoStr = handleDoubleQuotes(echoStr);
    // }
    else if (hasConsecutiveSpaces(echoStr)){
      // consecutive spaces are collasped unless quoted
      size_t firstSpace = echoStr.find(' ');
      size_t lastSpace = echoStr.find_last_of(' ');
      echoStr = echoStr.substr(0, firstSpace) + echoStr.substr(lastSpace, echoStr.length() - lastSpace);
    }

    cout << echoStr << endl;
  }

  void type(string& input) {
    int start = input.find(' ') + 1;
    int end = input.length();
    int sizeOfTypeStr = end - start;

    string typeArg = input.substr(start, sizeOfTypeStr);
    vector<string> builtIns {"echo", "exit", "type", "pwd", "cd"};

    if (find(builtIns.begin(), builtIns.end(), typeArg) != builtIns.end()) {
      cout << typeArg << " is a shell builtin" << endl;
    }
    else {
      searchPath(typeArg);
    }
  }