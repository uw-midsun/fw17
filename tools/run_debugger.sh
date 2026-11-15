#!/usr/bin/env bash
set -euo pipefail

# TODO(LorenzoG): Re-write in python once python is brought into the repository.

LOAD="${LOAD:-false}"
OPENOCD_URL="${OPENOCD_URL:-localhost:3333}"

if [[ $# -ne 1 ]]; then
    printf 'Error: expected one TARGET_BINARY argument.\n' >&2
    exit 2
fi

TARGET_BINARY="$1"

if [[ "$LOAD" != true && "$LOAD" != false ]]; then
    printf 'Error: LOAD must be true or false.\n' >&2
    exit 2
fi

GDB_ARGS=(
    -q
    -ex "target extended-remote $OPENOCD_URL"
    -ex "file $TARGET_BINARY"
)

if [[ "$LOAD" == true ]]; then
    GDB_ARGS+=(-ex load)
fi

arm-none-eabi-gdb "${GDB_ARGS[@]}"
