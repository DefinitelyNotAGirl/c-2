#!/usr/bin/env python3
# -*- coding:utf-8 -*-

compiler_cxx = "g++"
compiler_c = "gcc"
assembler = "as"
linker = "g++"

args_cxx = "-std=c++20 -g -Wno-write-strings -rdynamic -pedantic -Wno-multichar -Wunreachable-code -Wno-literal-suffix -Wno-pointer-arith -fpermissive -Iinc/ -g -c -fdiagnostics-color"
args_linker = "-Llib/ -lc -lstdc++ -lcargparse -lbacktrace -g -rdynamic"
