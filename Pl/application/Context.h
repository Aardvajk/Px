#ifndef CONTEXT_H
#define CONTEXT_H

#include "common/Object.h"

#include <pcx/args.h>
#include <pcx/datastream.h>

#include <vector>

class Context
{
public:
    Context(int argc, char *argv[], std::vector<std::string> &files);

    pcx::args args;

    std::vector<Object::Unit> units;

    pcx::data_ostringstream ds;
};

#endif // CONTEXT_H
