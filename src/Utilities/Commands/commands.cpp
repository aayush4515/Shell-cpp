#include "commands.h"
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <readline/readline.h>
#include <readline/history.h>

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
    // 1. skip the command name itself
    size_t firstSpace = input.find_first_of(" \t");
    if (firstSpace == string::npos) {                // user typed only “echo”
        std::cout << '\n';
        return;
    }

    string rawArgs = input.substr(firstSpace + 1);   // everything after “echo ”
    string echoStr = "";

    // check for backslash outside quotes
    if (hasBackslashOutsideQuotes(rawArgs)) {    // Does it have non-quoted backslash(es)?
      echoStr = processNonQuotedBackslashes(rawArgs);
    }
    else {
      echoStr = stripQuotesAndCollapse(rawArgs);
    }
    std::cout << echoStr << '\n';
}

void type(string& input) {
  int start = input.find(' ') + 1;
  int end = input.length();
  int sizeOfTypeStr = end - start;

  string typeArg = input.substr(start, sizeOfTypeStr);
  vector<string> builtIns {"echo", "exit", "type", "pwd", "cd", "history"};

  if (find(builtIns.begin(), builtIns.end(), typeArg) != builtIns.end()) {
    cout << typeArg << " is a shell builtin" << endl;
  }
  else {
    searchPath(typeArg);
  }
}

void history() {
  //displayHistory();

  // USING READLINE LIBRARY, DISPLAY HISTORY FROM MEMORY
  HIST_ENTRY** history = history_list();
  int i;

  if (history) {
    for (i = 0; history[i]; i++) {
      cout << '\t' << i + 1 << ' ' <<  history[i]->line << endl;
    }
  }
}

void history(int n) {
  //displayHistory(n);

  // USING READLINE LIBRARY, DISPLAY HISTORY FROM MEMORY
  HIST_ENTRY** history = history_list();
  int i;

  if (history) {
    for (i = history_length - n; history[i]; i++) {
      cout << '\t' << i + 1 << ' ' <<  history[i]->line << endl;
    }
  }
}