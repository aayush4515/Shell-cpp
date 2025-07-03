#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <sys/stat.h>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include "Utilities/Commands/commands.h"

using namespace std;
namespace fs = filesystem;

// REPL logic
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

      bool redirectStdout = false;
      string outRedirectPath;

      if (input.find('>') != string::npos || input.find("1>") != string::npos) {
        redirectStdout = true;

        // get the redirection path
        size_t start = input.find('>') + 2;                  // cmd > target: start is 2 positions after '>'
        size_t end = input.length();                         // end is the postion of last character
        outRedirectPath = input.substr(start, end - start);

        // trim the input to exlude outRedirectPath
        input = input.substr(0, start - 3);
      }

      // extract the command
      string command = extractCommand(input);

      // is it a built-in command?
      if (isBuiltin(command)) {
        int savedStdout = -1;
        int outFd       = -1;

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
                // 3) Redirect stdout → file
                if (dup2(outFd, STDOUT_FILENO) < 0) {
                    perror("dup2");  // failed to redirect
                }
                close(outFd);  // no longer needed
            }
        }

        // 4) Run the builtin; all std::cout now goes into `outRedirectPath`
        runBuiltin(command, input);

        if (redirectStdout) {
            // 5a) Flush C++/C buffers so nothing is left unwritten
            cout.flush();
            fflush(stdout);

            // 5b) Restore the original stdout
            if (dup2(savedStdout, STDOUT_FILENO) < 0) {
                perror("dup2 restore");
            }
            close(savedStdout);
        }
      }
      // is it an external exe command?
      else if (isExternalExecutableCommand(command)) {
        if (redirectStdout) {
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
                // CHILD: open file, dup2, execvp
                int fd = open(
                    outRedirectPath.c_str(),
                    O_CREAT | O_TRUNC | O_WRONLY,
                    0644
                );
                if (fd < 0) {
                    perror("open for stdout");
                    _exit(1);
                }
                if (dup2(fd, STDOUT_FILENO) < 0) {
                    perror("dup2 stdout");
                    _exit(1);
                }
                close(fd);

                execvp(argv[0], argv.data());
                // if execvp returns, it failed
                perror("execvp");
                _exit(1);
            }
            else {
                // PARENT: wait for the child to finish
                int status;
                if (waitpid(pid, &status, 0) < 0) {
                    perror("waitpid");
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
