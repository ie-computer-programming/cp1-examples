#!/usr/bin/env sh
# Runs every built example and reports its exit code.
# Several of these fail on purpose -- that is the point, so failures do not
# stop the run.
set -u

BUILD="${1:-build}"

if [ ! -d "$BUILD" ]; then
    echo "no build directory at '$BUILD' -- run:" >&2
    echo "  cmake -S . -B build && cmake --build build" >&2
    exit 2
fi

# 06-* take a path; an empty file drives them down their error path.
EMPTY="$BUILD/empty.txt"
: > "$EMPTY"

for exe in "$BUILD"/0*; do
    [ -f "$exe" ] && [ -x "$exe" ] || continue
    name=$(basename "$exe")

    printf '\n===== %s =====\n' "$name"
    case "$name" in
        06-*) "$exe" "$EMPTY" ;;
        *)    "$exe" ;;
    esac
    printf '[exit code: %d]\n' "$?"
done

printf '\n'
