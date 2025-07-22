#!/bin/bash

# === Prompt for contributor name ===
AUTHOR=$(git config user.name)
TODAY=$(date "+%m/%d/%Y")

echo "[INFO] Detected username: $AUTHOR"

# === Project configuration ===
SERVER_OUT="server"
CLIENT_OUT="client"
COMMON_FILES="src/utils/log.c src/utils/hashTable.c"
PATTERN_AUTHOR="^( \* *Last Updating Author *: *).*"
PATTERN_DATE="^( \* *Last Update *: *).*"

# === Get list of modified source/header files ===
modified_files=$(git diff --name-only HEAD | grep -E '\.c$|\.h$')
if [[ -z "$modified_files" ]]; then
    modified_files=$(git diff --name-only HEAD~1 | grep -E '\.c$|\.h$')
fi

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
gcc -o "$SERVER_OUT" src/server/server.c $COMMON_FILES -lpthread
if [[ $? -eq 0 ]]; then
    echo "[SUCCESS] Server build complete. Output: $SERVER_OUT"
else
    echo "[ERROR] Server compilation failed."
fi

# === Compile the client ===
echo "[INFO] Compiling client..."
gcc -o "$CLIENT_OUT" src/utils/client.c $COMMON_FILES -lpthread
if [[ $? -eq 0 ]]; then
    echo "[SUCCESS] Client build complete. Output: $CLIENT_OUT"
else
    echo "[ERROR] Client compilation failed."
fi
