#ifndef CONTEXT_H
#define CONTEXT_H

#include "common/Object.h"
#include "common/FileMap.h"

#include <pcx/args.h>
#include <pcx/datastream.h>
#include <pcx/ptr_vector.h>

#include <vector>

class Context
{
public:
    Context(int argc, char *argv[], std::vector<std::string> &files);

    Object::Entity *find(const std::string &name);

    std::size_t dataPosition() const;

    pcx::args args;

    pcx::ptr_vector<Object::Unit> units;

    pcx::data_ostringstream ds;
    pcx::data_ostringstream cs;

    FileMap dm;
    FileMap cm;
};

#endif // CONTEXT_H
