#include "autocompletion.h"
using namespace std;

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

char* command_generator(const char* text, int state)
{
    // some built-in commands
    vector<string> commands = {"cd", "ls", "exit", "echo", "type", "mkdir", "help"};

    // append external commands to commands vector
    vector<string> externalCommands = extractExternalCommands();
    for (const auto& cmd : externalCommands) {
        commands.push_back(cmd);
    }

    size_t index, len;

    if (!state) {                        // first call
        index = 0;
        len   = strlen(text);
    }

    while (index < commands.size()) {
        const string& name = commands[index++];
        /* compare the first `len` characters with the user’s prefix */
        if (name.compare(0, len, text, len) == 0) {
            return strdup(name.c_str());   // Readline will free it
        }
    }
    return nullptr;
}

char** completer(const char* text, int start, int end)
{
    /* If we are at the start of the line, complete shell commands.
       Otherwise fall back to default filename completion.              */
    if (start == 0)
        return rl_completion_matches(text, command_generator);

    return nullptr;   // use default
}