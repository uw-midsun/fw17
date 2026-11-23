#!/usr/bin/env bash

set -euo pipefail

GIT_REPO_ROOT=$(git rev-parse --show-toplevel)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly SCRIPT_DIR GIT_REPO_ROOT

echo "Installing git hooks from ${SCRIPT_DIR} into ${GIT_REPO_ROOT} :)"

ln -s "${SCRIPT_DIR}/pre-commit.sh" "${GIT_REPO_ROOT}/.git/hooks/pre-commit"
