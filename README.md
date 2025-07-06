# 🐚 Custom Shell in C++

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

shell-project/
│
├── main.cpp # Entry point with REPL
├── commands/ # Built-in command implementations
│ ├── echo.cpp
│ ├── cd.cpp
│ └── ...
├── utilities/ # Helpers: history, parser, autocompletion
├── headers/ # Header files for modularity
├── Makefile # Build script
└── README.md # This file

---

## 🛠️ How to Run

### 📦 Prerequisites

- C++ compiler (`g++`)
- `readline` library installed (`sudo apt install libreadline-dev` on Debian/Ubuntu)

### 🧪 Build and Run

```bash
git clone https://github.com/yourusername/custom-shell-cpp.git
cd custom-shell-cpp
make
./myshell
```

## 🙏 Acknowledgements
This shell was developed as part of a systems programming learning path inspired by Codecrafters' "Build Your Own Shell" challenge. Special thanks to open-source communities and documentation for insights into readline, shell parsing, and POSIX features.