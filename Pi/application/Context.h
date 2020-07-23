#ifndef CONTEXT_H
#define CONTEXT_H

#include "common/FileMap.h"

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "syms/SymStack.h"

#include "components/Global.h"
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

    std::size_t currentPosition() const;

    pcx::args args;

    SourceList sources;
    Scanner scanner;

    SymStack syms;

    pcx::indexed_set<std::string> strings;
    pcx::ptr_vector<Global> globals;
    pcx::ptr_vector<Function> functions;

    FileMap dm;
    FileMap cm;
};

#endif // CONTEXT_H
