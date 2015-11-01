#!/bin/bash

# Change this to point to your GCC build directory:
GCC_BUILD_DIR="$HOME/gcc/build"

# C compiler
CC1="$GCC_BUILD_DIR/gcc/cc1"
# C++ compiler
CC1PLUS="$GCC_BUILD_DIR/gcc/cc1plus"
# Fortran compiler
F951="$GCC_BUILD_DIR/gcc/f951"

# Directory containing this script
SCRIPT_DIR="$(dirname "$0")"

$CC1 $SCRIPT_DIR/simple.c -quiet -o /dev/null
$CC1 $SCRIPT_DIR/include.c -quiet -o /dev/null
$CC1 $SCRIPT_DIR/include.i -quiet -o /dev/null
$CC1 $SCRIPT_DIR/macro.c -quiet -o /dev/null
$CC1 $SCRIPT_DIR/printf.c -Wall -quiet -o /dev/null
# This option is only available in GCC 6, but you should use trunk revision anyway
$CC1 -Wmisleading-indentation $SCRIPT_DIR/indent.c -quiet -o /dev/null
$CC1PLUS $SCRIPT_DIR/overload.cc -quiet -o /dev/null
$CC1PLUS $SCRIPT_DIR/template.cc -quiet -o /dev/null
# If Fortran compiler exists, use it
[[ -e $F951 ]] && $F951 $SCRIPT_DIR/test.f95 -quiet -o /dev/null

