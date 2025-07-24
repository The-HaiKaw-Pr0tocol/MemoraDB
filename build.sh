#!/bin/bash

# === Ensure script runs from project root ===
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"
PROJECT_ROOT=$(git rev-parse --show-toplevel)
cd "$PROJECT_ROOT"
echo "[INFO] Working directory set to project root: $(pwd)"

# === Prompt for contributor name ===
AUTHOR=$(git config user.name)
TODAY=$(date "+%m/%d/%Y")
echo "[INFO] Detected username: $AUTHOR"

# === Project configuration ===
SERVER_OUT="server"
CLIENT_OUT="client"
UTILS_FILES=$(find src/utils -name '*.c')
PARSER_FILES=$(find src/parser -name '*.c')
PATTERN_AUTHOR="^( \* *Last Updating Author *: *).*"
PATTERN_DATE="^( \* *Last Update *: *).*"

# === Get list of modified/added/renamed source/header files ===
modified_files=$(git status --porcelain | awk '{print $2}' | grep -E '\.c$|\.h$')

# === Update headers in each modified file ===
echo "[INFO] Updating file headers..."
for file in $modified_files; do
    if [[ -f "$file" ]]; then
        echo "→ Updating $file"
        sed -i -E "s#${PATTERN_AUTHOR}#\1${AUTHOR}#" "$file"
        sed -i -E "s#${PATTERN_DATE}#\1${TODAY}#" "$file"
    fi
done

# === Compile the server ===
echo "[INFO] Compiling server..."
gcc -o "$SERVER_OUT" src/server/server.c $UTILS_FILES $PARSER_FILES -lpthread
if [[ $? -eq 0 ]]; then
    echo "[SUCCESS] Server build complete. Output: $SERVER_OUT"
else
    echo "[ERROR] Server compilation failed."
fi

# === Compile the client ===
echo "[INFO] Compiling client..."
gcc -o "$CLIENT_OUT" src/client/client.c $UTILS_FILES -lpthread
if [[ $? -eq 0 ]]; then
    echo "[SUCCESS] Client build complete. Output: $CLIENT_OUT"
else
    echo "[ERROR] Client compilation failed."
fi
