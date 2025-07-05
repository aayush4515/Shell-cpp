#include "helpers.h"
#include <cstdlib>
#include <filesystem>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <sys/stat.h>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>

#include "../Commands/commands.h"
#include "../Autocompletion/autocompletion.h"

namespace fs = filesystem;

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

bool hasPipe(string& input) {
  if (input.find('|') != string::npos) {
    bool inSingle = false;
    bool inDouble = false;

    for (char ch : input) {
      if (ch == '\'' && !inSingle) {
        inSingle = !inSingle;
      } else if (inSingle && ch == '|') {
        return false;
      }

      if (ch == '"' && !inDouble) {
        inDouble = !inDouble;
      } else if (inDouble && ch == '|') {
        return false;
      }
    }
    return true;
  }
  return false;
}

vector<string> splitOnPipe(const string& input) {
    vector<std::string> result;
    stringstream ss(input);
    string segment;

    while (getline(ss, segment, '|')) {
        // Trim leading/trailing spaces
        segment.erase(segment.begin(), find_if(segment.begin(), segment.end(), [](int ch) {
            return !isspace(ch);
        }));
        segment.erase(find_if(segment.rbegin(), segment.rend(), [](int ch) {
            return !isspace(ch);
        }).base(), segment.end());

        result.push_back(segment);
    }

    return result;
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

bool isBuiltin(const string& cmd) {
  if (cmd == "echo" || cmd == "cd" || cmd == "type" || cmd == "pwd" || cmd == "exit") {
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

// extracts all external commands from $PATH
vector<string> extractExternalCommands() {
  vector<string> dirs = extractPath();
  vector<string> externalCommands;
  char* currFile = NULL;

  for (const auto& dir : dirs) {
      // dir_ptr to open directory and dirent to read directory
      DIR* dir_ptr = nullptr;
      struct dirent* read_dir;

      // open the directory
      dir_ptr = opendir(dir.c_str());
      // skip non-existent directories
      if (dir_ptr == nullptr) {
          continue;
      }
      // read every file in the directory
      while ((read_dir = readdir(dir_ptr)) != NULL) {
          // get current file name
          currFile = read_dir->d_name;

          // construct full path
          fs::path fullPath = fs::path(dir) / currFile;

          // check if the file is executable
          if (access(fullPath.c_str(), X_OK) == 0) {
              // the file is executable, add it to the external exe commands vector
              externalCommands.push_back(string(currFile));
          }
      }
      // close the directory
      closedir(dir_ptr);
  }
  return externalCommands;
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

void redirectOrAppend(string& input) {

}

void run(string& input) {
  bool hasRedirectionOrAppend = false;
  bool redirectStdout = false;
  bool redirectStderr = false;
  bool appendStdout = false;
  bool appendStderr = false;
  string outRedirectPath;
  string errRedirectPath;
  string appendOutPath;
  string appendErrPath;

  // Parse append stderr symbol
  {
    if (input.find("2>>") != string::npos) {
      appendStderr = true;

      // get the redirection path
      size_t start = input.find('>') + 3;                  // cmd 2>> target: start is 3 positions after first '>'
      size_t end = input.length();                         // end is the postion of last character
      appendErrPath = input.substr(start, end - start);
      hasRedirectionOrAppend = true;

      // trim the input to exlude appendOutPath
      input = input.substr(0, start - 4);
    }
  }

  // Parse append stdout symbol
  {
    if (!appendStderr && !redirectStdout && !redirectStderr) {
      if (input.find("1>>") != string::npos || input.find(">>") != string::npos) {
        appendStdout = true;

        // get the redirection path
        size_t start = input.find('>') + 3;                  // cmd 1>> target: start is 3 positions after first '>'
        size_t end = input.length();                         // end is the postion of last character
        appendOutPath = input.substr(start, end - start);
        hasRedirectionOrAppend = true;

        // trim the input to exlude appendOutPath
        input = input.substr(0, start - 4);
      }
    }
  }

  // Parse stderr redirection ("2> file")
  {
    if (!appendStderr && !appendStdout && !redirectStdout) {
      if (input.find("2>") != string::npos) {
        redirectStderr = true;

        // get the redirection path
        size_t start = input.find(">") + 2;                  // cmd > target: start is 2 positions after '>'
        size_t end = input.length();                         // end is the postion of last character
        errRedirectPath = input.substr(start, end - start);
        hasRedirectionOrAppend = true;

        // trim the input to exlude outRedirectPath
        input = input.substr(0, start - 3);
      }
    }
  }

  // Parse stdout redirection ("> file" or "1> file")
  {
    if (!appendStderr && !appendStdout && !redirectStderr) {
      if (input.find("1>") != string::npos || input.find('>') != string::npos)  {
        redirectStdout = true;

        // get the redirection path
        size_t start = input.find('>') + 2;                  // cmd > target: start is 2 positions after '>'
        size_t end = input.length();                         // end is the postion of last character
        outRedirectPath = input.substr(start, end - start);
        hasRedirectionOrAppend = true;

        // trim the input to exlude errRedirectPath
        input = input.substr(0, start - 3);
      }
    }
  }
  // extract the command
  string command = extractCommand(input);

  /*

      | ------------------- HANDLE PIPELINE ----------------------- |

  */

  if (hasPipe(input)) {
    vector<string> stages = splitOnPipe(input);
    int inFd = STDIN_FILENO;                      // read-end for current stage

    for (size_t i = 0; i < stages.size(); ++i) {
        string stageCmd = extractCommand(stages[i]);

        int pipefd[2]{-1,-1};
        if (i != stages.size() - 1 && pipe(pipefd) < 0) {
            perror("pipe");
            return;
        }

        pid_t pid = fork();
        if (pid == 0) {                           // ── CHILD ──
            dup2(inFd, STDIN_FILENO);             // read from prev. pipe
            if (i != stages.size() - 1)
                dup2(pipefd[1], STDOUT_FILENO);   // write to next pipe

            /* ▼ close fds we no longer need in the child */
            if (pipefd[0] != -1) close(pipefd[0]);   // read end of new pipe
            if (pipefd[1] != -1) close(pipefd[1]);   // (already duped)
            if (inFd != STDIN_FILENO) close(inFd);

            // run built-in or external
            if (isBuiltin(stageCmd)) {
                runBuiltin(stageCmd, stages[i]);
            }
            else {
                // cerr << stageCmd << ": builtin not found\n";
                cout << "here" << endl;
                cout << "external command: " << stageCmd << endl;
                //system(stageCmd.c_str());
                system(input.c_str());
            }
            _exit(0);                             // success
        }

        /* ── PARENT ── clean up and chain next stage */
        if (inFd != STDIN_FILENO) close(inFd);     // finished with prev read end
        if (i != stages.size() - 1) {
            close(pipefd[1]);                      // parent closes write end
            inFd = pipefd[0];                      // next stage reads here
        }
    }
    while (wait(nullptr) > 0) { }                 // reap all children
    return;
  }


  // is it a built-in command?
  // if (isBuiltin(command)) {
    if (hasRedirectionOrAppend) {
    /*

      | ------------------- HANDLE REDIRECTIONS ----------------------- |

    */

    int savedStdout = -1, savedStderr = -1;
    int outFd       = -1, errFd = -1, appendFd = -1, appendErrFd = -1;

    // Redirect stdout if requested
    if (redirectStdout) {
      // 1) Save the real stdout
      savedStdout = dup(STDOUT_FILENO);
      if (savedStdout < 0) {
          perror("dup");  // failed to save stdout
      }

      // 2) Open (or create) the target file
      outFd = open(
        outRedirectPath.c_str(),
        O_CREAT | O_TRUNC | O_WRONLY,
        0644
      );
      if (outFd < 0) {
          perror("open");  // failed to open file
      } else {
          // 3) Redirect stdout -> file
          if (dup2(outFd, STDOUT_FILENO) < 0) {
              perror("dup2");  // failed to redirect
          }
          close(outFd);  // no longer needed
      }
    }

    // Redirect stderr if requested
    if (redirectStderr) {
      // save the real stderr
      savedStderr = dup(STDERR_FILENO);
      if (savedStderr < 0) {
        perror("dup");  // failed to save stderr
      }

      // open or create the target file
      errFd = open(errRedirectPath.c_str(),
                  O_CREAT | O_TRUNC | O_WRONLY, 0644);
      if (errFd < 0) {
        perror("open"); // failed to open file
      } else {
        // redirect stderr -> file
        if (dup2(errFd, STDERR_FILENO) < 0) {
          perror("dup2");   // failed to redirect
        }
        close(errFd);   // no longer needed
      }

    }

    // process stdout append if requested
    if (appendStdout) {
      savedStdout = dup(STDOUT_FILENO);
      if (savedStdout < 0) {
        perror("dup");
      }
      appendFd = open(appendOutPath.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);
      if (appendFd < 0) {
        perror("append");
      } else {
        if (dup2(appendFd, STDOUT_FILENO) < 0) {
          perror("dup2");
        }
        close(appendFd);
      }
    }

    // process stderr append if requested
    if (appendStderr) {
      savedStderr = dup(STDERR_FILENO);
      if (savedStderr < 0) {
        perror("dup");
      }

      appendErrFd = open(appendErrPath.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);
      if (appendErrFd < 0) {
        perror("append");
      } else {
        if (dup2(appendErrFd, STDERR_FILENO) < 0) {
          perror("dup2");
        }
        close(appendErrFd);
      }
    }

    // 4) Run the builtin; all cout and cerr are redirected to the respective files
    if (redirectStdout || redirectStdout || appendStdout || appendStderr) {
      runBuiltin(command, input);
    }

    // Flush and restore stdout and stderr
    if (redirectStdout) {
        // 5a) Flush C++/C buffers so nothing is left unwritten
        cout.flush();
        fflush(stdout);

        // 5b) Restore the original stdout
        if (dup2(savedStdout, STDOUT_FILENO) < 0) {
            perror("restore stdout");
        }
        close(savedStdout);
    }
    if (redirectStderr) {
      fflush(stderr);
      if (dup2(savedStderr, STDERR_FILENO) < 0) {
        perror("restore stderr");
      }
      close(savedStderr);
    }
    if (appendStdout) {
      cout.flush();
      fflush(stdout);

      if (dup2(savedStdout, STDOUT_FILENO) < 0) {
        perror("append stdout");
      }
      close(savedStdout);
    }
    if (appendStderr) {
      // cout << "Appending stderr" << endl;
      fflush(stderr);

      if (dup2(savedStderr, STDERR_FILENO) < 0) {
        perror("append stderr");
      }
      close(savedStderr);
    }

    // run builtin if none of the above conditions meet
    //runBuiltin(command, input);

  }
  // is it an external exe command?
  else if (isBuiltin(command)) {
    runBuiltin(command, input);
  }
  else if (isExternalExecutableCommand(command)) {
    if (redirectStdout || redirectStderr || appendStdout || appendStderr) {
        // 1) Split the trimmed input into arguments
        std::istringstream iss(input);
        std::vector<std::string> parts;
        std::string tok;
        while (iss >> tok) {
            parts.push_back(tok);
        }

        // 2) Build argv[] for execvp
        std::vector<char*> argv;
        for (auto &s : parts) {
            argv.push_back(s.data());
        }
        argv.push_back(nullptr);

        // 3) Fork a child
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
        }
        else if (pid == 0) {
          // CHILD: set up redirections
          if (redirectStdout) {
            int fd = open(outRedirectPath.c_str(),
                          O_CREAT | O_TRUNC | O_WRONLY, 0644);
            if (fd < 0) {
              perror("open stdout");
              _exit(1);
            }
            if (dup2(fd, STDOUT_FILENO) < 0) {
              perror("dup2 stdout");
              _exit(1);
            }
            close(fd);
          }
          if (redirectStderr) {
            //cout << "Redirecterr path: " << errRedirectPath << endl;
            int fd = open(errRedirectPath.c_str(),
                          O_CREAT | O_TRUNC | O_WRONLY, 0644);
            if (fd < 0) {
              //perror("open stderr");
              _exit(1);
            }
            if (dup2(fd, STDERR_FILENO) < 0) {
              perror("dup2 stderr");
              _exit(1);
            }
            close(fd);
          }
          if (appendStdout) {
            int fd = open(appendOutPath.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);

            if (fd < 0) {
              _exit(1);
            }
            if(dup2(fd, STDOUT_FILENO) < 0) {
              perror("dup2 stdout");
              _exit(1);
            }
            close(fd);
          }
          if (appendStderr) {
            int fd = open(appendErrPath.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);

            if (fd < 0) {
              _exit(1);
            }
            if(dup2(fd, STDERR_FILENO) < 0) {
              perror("dup2 stderr");
              _exit(1);
            }
            close(fd);
          }

          // Execute the command
          execvp(argv[0], argv.data());
          // if execvp returns, it failed
          perror("execvp");
          _exit(1);
        }
        else {
          // PARENT: wait
          // int status;
          // if (waitpid(pid, &status, 0) < 0) {
          //     perror("waitpid");
          // }
          if (wait(0) == 1) {
            perror("wait");
          }
        }
    }
    else {
        // no redirection: just hand off to /bin/sh
        system(input.c_str());
    }
  }

  // output as invalid command
  else {
    cout << input << ": command not found" << endl;
  }
}