#ifndef CONTEXT_H
#define CONTEXT_H

#include "common/Object.h"

#include <pcx/args.h>
#include <pcx/ptr_vector.h>
#include <pcx/datastream.h>

class Context
{
public:
    Context(int argc, char *argv[], std::vector<std::string> &files);

    Object::Entity *find(const std::string &name);

    pcx::args args;

    pcx::ptr_vector<Object::File> files;

    pcx::data_ostringstream ds;
    pcx::data_ostringstream cs;
};

#endif // CONTEXT_H
