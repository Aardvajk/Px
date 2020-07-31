#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "syms/SymTree.h"

#include "types/TypeCache.h"

#include "generics/GenericStack.h"

#include "infos/ParseInfo.h"
#include "infos/FuncInfo.h"

#include <pcx/args.h>
#include <pcx/ptr_vector.h>

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
    pcx::ptr_vector<FuncInfo> funcInfos;
};

#endif // CONTEXT_H
