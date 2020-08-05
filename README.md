# Px
VM/Compiler Project

Essentially a small toy version of LLVM, intended purely for fun.

**pc** - high-level language compiler, reads .pc text files, generates .pi text files\
**pi** - intermediate language compiler, reads .pi files, generates .po object files and .pmap debugging information files\
**pl** - linker, takes a number of .po files and generates a .px executable\
**pv** - virtual machine, reads .px files and executes them\
**pd** - reads .po or .px files, and .pmap files if present, and produces a human-readable text output\
**pb** - simple build system, reads a batch file and executes commands

```
namespace std
{

func print(:std.int):std.null;
func print(:std.char):std.null;

}

func<T> foo(a:T):std.null
{
    std.print(a);
}

func<T> bar(a:T):T
{
    return a;
}

func main():std.null
{
    foo<std.int>(bar<std.int>(123));
    foo<std.char>(bar<std.char>('S'));
}
```