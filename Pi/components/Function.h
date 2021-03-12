#ifndef FUNCTION_H
#define FUNCTION_H

#include <pcx/datastream.h>

#include <cstddef>

class Sym;

class Function
{
public:
    Function(Sym *sym, std::size_t id) : sym(sym), id(id), args(0) { }

    Sym *sym;
    std::size_t id;

    std::size_t args;

    pcx::data_ostringstream bytes;
};

#endif // FUNCTION_H
