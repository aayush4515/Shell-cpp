#!/usr/bin/env bash
# build.sh – simple one-shot build for the project

set -euo pipefail          # safer bash settings: abort on errors & unset vars

# --- Optional: create a separate output folder ------------------------------
OUT_DIR="src"

# --- Compile ---------------------------------------------------------------
g++ -std=c++17 \
    -I/usr/local/opt/readline/include \
    src/main.cpp \
    src/Utilities/Commands/commands.cpp \
    src/Utilities/EchoHelpers/echohelpers.cpp \
    src/Utilities/Helpers/helpers.cpp \
    src/Utilities/Autocompletion/autocompletion.cpp \
    -L/usr/local/opt/readline/lib \
    -lreadline \
    -lncurses \
    -o "$OUT_DIR/main"

echo -e "\n✅ Build succeeded → $OUT_DIR/main"
