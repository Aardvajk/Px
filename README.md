# Px
VM/Compiler Project

Essentially a small toy version of LLVM, intended purely for fun.

**pi** - intermediate language compiler, reads .pi files, generates .po object files and .pmap debugging information files\
**pl** - linker, takes a number of .po files and generates a .px executable\
**pv** - virtual machine, reads .px files and executes them\
**pd** - reads .po or .px files, and .pmap files if present, and produces a human-readable text output\
**pb** - simple build system, reads a batch file and executes commands
