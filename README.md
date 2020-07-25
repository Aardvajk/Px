# Px
VM/Compiler Project

A toy virtual machine and compiler project consisting of a virtual machine that runs a very small set of bytecode, an intermediate language that generates object files and a linker that constructs an "executable" for the machine.

Essentially a small toy version of LLVM, intended purely for fun.

**pc** - high-level language compiler, reads .pc text files, generates .pi text files\
**pi** - intermediate language compiler, reads .pi files, generates .po object files and .pmap debugging information files\
**pl** - linker, takes a number of .po files and generates a .px executable\
**pd** - debugger, reads .po or .px files, and .pmap files if present, and produces a human-readable text output\
**pb** - simple build system, reads a batch file and executes commands\
