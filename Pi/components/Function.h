#ifndef FUNCTION_H
#define FUNCTION_H

#include "common/Object.h"

#include <pcx/datastream.h>

#include <cstddef>
#include <vector>

class Sym;

class Function
{
public:
    Function(Sym *sym, std::size_t id) : sym(sym), id(id), vars(0), args(0) { }

    Sym *sym;
    std::size_t id;

    std::size_t vars;
    std::size_t args;

    std::vector<Object::Link> links;

    pcx::data_ostringstream bytes;
};

#endif // FUNCTION_H
