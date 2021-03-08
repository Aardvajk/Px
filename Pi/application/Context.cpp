#include "Context.h"

#include "framework/Error.h"

Context::Context(int argc, char *argv[], std::vector<std::string> &files) : args(argc, argv, files)
{
}

void Context::open(const std::string &path)
{
    throw Error("not implemented");
}
