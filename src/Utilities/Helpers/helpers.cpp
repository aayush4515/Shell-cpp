#include "helpers.h"

#include "../Commands/commands.h"

namespace fs = filesystem;

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

bool isBuiltin(const string& cmd) {
  if (cmd == "echo" || cmd == "cd" || cmd == "type" || cmd == "pwd") {
    return true;
  }
  return false;
}

void runBuiltin(string& cmd, string& input) {
  // check for various commands
  if (cmd == "echo") {
    echo(input);
    return;
  }
  else if (cmd == "type") {
    type(input);
    return;
  }
  else if (cmd == "pwd") {
    pwd();
    return;
  }
  else if (cmd == "cd") {
    cd(input);
    return;
  }
}

// extracts the path and returns splitted directories in a vector
vector<string> extractPath () {
  // stores the directories from the path variables
  vector<string> paths;

  // extracting the path variable
  const char* envVariable = "PATH";
  string path = string(getenv(envVariable));

  // splitting the path variables into directories and storing in the vector pathDirectories

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
  // searches every single directory in the path for the given commands

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
                string displayPath = string(entry.path()).substr(0, string(entry.path()).length());
                cout << targetFile << " is " << displayPath << endl;
              }
              else {
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
  cout << targetFile << ": not found" << endl;
}

bool isExternalExecutableCommand(const string& cmd) {
  vector<string> dirs = extractPath();

  // ── walk each directory from $PATH
  for (const string& dir : dirs) {

      // Build full path: <dir>/<cmd>
      fs::path candidate = fs::path(dir) / cmd;

      // Is it a regular file AND executable by someone?
      if (fs::exists(candidate) && fs::is_regular_file(candidate)) {
          fs::perms p = fs::status(candidate).permissions();
          if ((p & fs::perms::owner_exec)  != fs::perms::none ||
              (p & fs::perms::group_exec)  != fs::perms::none ||
              (p & fs::perms::others_exec) != fs::perms::none) {
              return true;                  // found a runnable command
          }
      }
  }
  return false;                             // nothing matched
}

string extractCommand(const string& input) {
  int start = 0;
  int end = input.find(' ');
  int sizeCmd = end - start;
  string command = input.substr(start, sizeCmd);

  return command;
}