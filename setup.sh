#!/usr/bin/env bash
set -eu

DIRPATH="$(dirname "$(realpath "$0")")"
LIBPATH="${DIRPATH}/lib"
BINPATH="${DIRPATH}/bin"

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${LIBPATH}"
export PATH="${PATH}:${BINPATH}"
