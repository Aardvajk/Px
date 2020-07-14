# Px
VM/Compiler Project

A toy virtual machine and compiler project consisting of a virtual machine that runs a very small set of bytecode, an intermediate language that generates object files and a linker that constructs an "executable" for the machine.

My name is Paul, so everything starts with a 'P'. Pi is the intermediate language compiler. Pl is the linker. Pv is the virtual machine and Pd is the debugger or dumper that outputs a human-friendly text description of the bytecode files (.po and .px).

Pc is the compiler for the higher level language. The idea is that Pc reads .pc text files and generates .pi files that are then compiled into .po files by Pi. Pl then takes a collection of .po files and generates a .px file that can be executed by Pv.

Essentially a small toy version of LLVM, intended purely for fun.
