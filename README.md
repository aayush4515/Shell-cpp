<div id="top">
<!-- BADGES -->
<img src="https://img.shields.io/github/last-commit/aayush4515/Shell-cpp?style=flat&logo=git&logoColor=white&color=0080ff" alt="last-commit">
<img src="https://img.shields.io/github/languages/top/aayush4515/Shell-cpp?style=flat&color=0080ff" alt="repo-top-language">
<img src="https://img.shields.io/github/languages/count/aayush4515/Shell-cpp?style=flat&color=0080ff" alt="repo-language-count">

<img src="https://img.shields.io/badge/GNU%20Bash-4EAA25.svg?style=flat&logo=GNU-Bash&logoColor=white" alt="GNU%20Bash">
<img src="https://img.shields.io/badge/C++-00599C.svg?style=flat&logo=C++&logoColor=white" alt="C++">
<img src="https://img.shields.io/badge/CMake-064F8C.svg?style=flat&logo=CMake&logoColor=white" alt="CMake">

</div>


# 🐚 Custom UNIX Shell in C++

A fully functional shell built from scratch using C++. This project mimics Unix-like shell behavior, supporting built-in commands, execution of external programs, history persistence, piping, redirection, autocompletion, and more.

---

## 🚀 Overview

This shell project was developed as a deep dive into systems programming and the internals of command-line interpreters. It offers many core features found in standard shells like Bash or Zsh, while giving low-level control and learning opportunities using C++ and POSIX system calls.

---

## ✅ Features

### 🔧 Core Functionality
- Print prompt and REPL loop
- Command execution (built-in and external)
- Error handling for invalid inputs

### 📦 Built-ins
- `cd` (absolute, relative, and home)
- `pwd`, `exit`, `echo`, and `type`

### 📂 Navigation
- Absolute/relative/home directory support

### 🔁 Piping
- Dual and multi-command pipelines
- Piping with built-ins

### 📤 Redirection
- `>`, `>>` for `stdout`
- `2>`, `2>>` for `stderr`

### ⌨️ Autocompletion
- Supports built-ins, executables, arguments, and partial matches

### 📜 History
- Command history tracking with up/down arrow navigation
- Execution of previous commands
- Limiting history entries

### 💾 History Persistence
- Save/load history to/from file
- Append history on exit
- Load history on startup

---

## 🧰 Tech Stack

- **Language:** C++
- **Build System:** Make / g++
- **Libraries:** POSIX system calls, `readline` for history and autocompletion
- **OS:** Linux/macOS (Unix-based systems)

---

## 📁 Project Structure

``` sh
shell-cpp/
│
├── src/                # Main source folder
│ ├── Utilities/        # Utility modules
│ ├── Autocompletion/   # Tab completion logic
│ ├── Commands/         # Built-in command implementations
│ ├── EchoHelpers/      # Helpers specific to echo command
│ └── Helpers/          # General helper functions
│
├── main.cpp            # Entry point of the shell
├── CMakeLists.txt      # CMake build script
├── codecrafters.yml    # Codecrafters config
├── history.txt         # History persistence file
├── README.md           # Project documentation
├── run.sh              # Shell launch script
├── your_program.sh     # Program test wrapper
├── vcpkg.json          # Vcpkg dependency manifest
├── vcpkg-configuration.json # Vcpkg config file
├── .gitattributes
├── .gitignore
```

---

## 🛠️ How to Run

### 📦 Prerequisites

- C++ compiler (`g++`)
- `readline` library installed (`sudo apt install libreadline-dev` on Debian/Ubuntu)

### 🧪 Build and Run

```bash
git clone https://github.com/aayush4515/Shell-cpp.git
cd Shell-cpp
./run.sh
```

## 🙏 Acknowledgements
This shell was developed as part of a systems programming learning path inspired by Codecrafters' [Build Your Own Shell](https://app.codecrafters.io/courses/shell/overview) challenge. Special thanks to open-source communities and documentation for insights into readline, shell parsing, and POSIX features.