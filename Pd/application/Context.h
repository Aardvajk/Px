#ifndef CONTEXT_H
#define CONTEXT_H

#include <pcx/args.h>

class Context
{
public:
    Context(int argc, char *argv[], std::vector<std::string> &files) : args(argc, argv, files) { }

    pcx::args args;
};

#endif // CONTEXT_H
