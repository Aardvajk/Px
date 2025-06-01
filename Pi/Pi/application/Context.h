#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"

#include <pcx/args.h>

class Context
{
public:
    Context(int argc, char *argv[], std::vector<std::string> &files);

    pcx::args args;

    SourceList sources;
};

#endif // CONTEXT_H
