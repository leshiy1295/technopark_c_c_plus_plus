#!/bin/bash

# Directory containing this script
SCRIPT_DIR="$(dirname "$0")"
# Compiler flags
FLAGS='-c -o /dev/null'

# You might also want to try this:
# FLAGS="$FLAGS -fdiagnostics-parseable-fixits -fdiagnostics-print-source-range-info"

clang $FLAGS $SCRIPT_DIR/simple.c
clang $FLAGS $SCRIPT_DIR/include.c
clang $FLAGS $SCRIPT_DIR/macro.c
clang $FLAGS -Wall $SCRIPT_DIR/printf.c
clang++ $FLAGS $SCRIPT_DIR/overload.cc
clang++ -std=c++14 $FLAGS $SCRIPT_DIR/template.cc

