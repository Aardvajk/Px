#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "syms/SymTree.h"

#include "types/TypeCache.h"

#include "generics/GenericStack.h"
#include "generics/GenericRequestSet.h"

#include "components/ParseInfo.h"
#include "components/FuncInfo.h"

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

    ParseInfo parseInfo;

    SymTree tree;
    TypeCache types;

    GenericStack generics;
    GenericRequestSet genericRequests;

    pcx::ptr_vector<FuncInfo> funcInfos;
};

#endif // CONTEXT_H
