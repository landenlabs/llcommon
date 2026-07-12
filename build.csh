#!/bin/csh -f
# Standalone syntax/link sanity check for llcommon - not a real build product.
# Compiles the shared sources into a static library so a `git submodule` update
# alone (with no consuming project) can still be verified to compile & link.
rm -f libllcommon.a
g++ -g -std=c++17 -Wall -c directory.cpp parseutil.cpp signals.cpp
ar rcs libllcommon.a directory.o parseutil.o signals.o
rm -f *.o
echo "libllcommon.a built OK"
