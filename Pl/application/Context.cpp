#include "Context.h"

Context::Context(int argc, char *argv[], std::vector<std::string> &files) : args(argc, argv, files)
{
}

Object::Entity *Context::find(const std::string &name)
{
    for(auto &f: files)
    {
        for(auto &e: f.entities)
        {
            if(f.strings[e.id] == name)
            {
                return &e;
            }
        }
    }

    return nullptr;
}
