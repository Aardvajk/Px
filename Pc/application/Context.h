#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "syms/SymTree.h"

#include "types/TypeCache.h"

#include "generics/GenericStack.h"

#include "infos/ParseInfo.h"

#include <pcx/args.h>

class Context
{
public:
    Context(int argc, char *argv[], std::vector<std::string> &files);

    void open(const std::string &path);

    pcx::args args;

    SourceList sources;
    Scanner scanner;

    SymTree tree;
    TypeCache types;
    GenericStack generics;

    ParseInfo parseInfo;
};

#endif // CONTEXT_H
