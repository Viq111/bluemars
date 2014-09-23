BlueMars - A modular procedural terrain generator
========
## How to build on Linux
- Install CMake and tools needed to compile/debug (g++, gdb, make, etc...)
- Run
  cmake .
  # or if you want the debug (-g) tag for gdb
  cmake -DCMAKE_BUILD_TYPE=Debug .
  make 
  #with -j 4 if you want to use 4 cpus
- Done
