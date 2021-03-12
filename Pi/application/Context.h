#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "syms/SymStack.h"

#include "components/Function.h"

#include <pcx/args.h>
#include <pcx/indexed_set.h>
#include <pcx/ptr_vector.h>

class Context
{
public:
    Context(int argc, char *argv[], std::vector<std::string> &files);

    void open(const std::string &path);

    Function &func(){ return functions.back(); }

    pcx::args args;

    SourceList sources;
    Scanner scanner;

    SymStack syms;

    pcx::indexed_set<std::string> strings;
    pcx::ptr_vector<Function> functions;
};

#endif // CONTEXT_H
