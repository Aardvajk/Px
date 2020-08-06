#ifndef GLOBAL_H
#define GLOBAL_H

#include "common/Object.h"

#include <pcx/datastream.h>

#include <cstddef>

class Sym;

class Global
{
public:
    Global(Sym *sym, Object::Entity::Flags flags, std::size_t id) : sym(sym), flags(flags), id(id) { }

    Sym *sym;
    Object::Entity::Flags flags;
    std::size_t id;

    pcx::data_ostringstream bytes;
};

#endif // GLOBAL_H
