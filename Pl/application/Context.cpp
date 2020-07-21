#include "Context.h"

Context::Context(int argc, char *argv[], std::vector<std::string> &files) : args(argc, argv, files)
{
}

Object::Entity *Context::find(const std::string &name)
{
    for(auto &u: units)
    {
        for(auto &e: u.entities)
        {
            if(u.strings[e.id] == name)
            {
                return &e;
            }
        }
    }

    return nullptr;
}
