#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "syms/SymTree.h"

#include "types/TypeCache.h"

#include "components/FuncInfo.h"
#include "components/TemplateFuncReq.h"

#include <pcx/args.h>
#include <pcx/ptr_vector.h>

#include <vector>
#include <unordered_map>

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

    std::vector<Sym::Type> containers;

    pcx::ptr_vector<FuncInfo> funcInfos;

    std::vector<std::unordered_map<std::string, Type*> > templateParams;
    std::vector<TemplateFuncReq> templateFuncReqs;
};

#endif // CONTEXT_H
