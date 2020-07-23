#ifndef CONTEXT_H
#define CONTEXT_H

#include "common/FileMap.h"

#include <pcx/args.h>

class Context
{
public:
    Context(int argc, char *argv[], std::vector<std::string> &files);

    pcx::args args;
    FileMap dm;
};

#endif // CONTEXT_H
