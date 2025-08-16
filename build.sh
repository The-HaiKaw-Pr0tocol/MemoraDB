#!/bin/bash
set -euo pipefail

# === Ensure script runs from project root ===
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"
PROJECT_ROOT=$(git rev-parse --show-toplevel)
cd "$PROJECT_ROOT"
echo "[INFO] Working directory set to project root: $(pwd)"

# === Contributor metadata ===
AUTHOR=$(git config user.name)
TODAY=$(date "+%m/%d/%Y")
echo "[INFO] Detected username: $AUTHOR"

# === Regex patterns ===
PATTERN_AUTHOR="^( \* *Last Updating Author *: *).*"
PATTERN_DATE="^( \* *Last Update *: *).*"

# === Modified/added/renamed source/header files ===
modified_files=$(git status --porcelain | awk '{print $2}' | grep -E '\.c$|\.h$' || true)

# === Update headers ===
echo "[INFO] Updating file headers..."
for file in $modified_files; do
    if [[ -f "$file" ]]; then
        echo "→ Updating $file"
        sed -i -E "s#${PATTERN_AUTHOR}#\1${AUTHOR}#" "$file"
        sed -i -E "s#${PATTERN_DATE}#\1${TODAY}#" "$file"
    fi
done
