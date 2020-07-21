#ifndef FUNCTION_H
#define FUNCTION_H

#include <pcx/datastream.h>

#include <cstddef>

class Sym;

class Function
{
public:
    Function(Sym *sym, std::size_t id) : sym(sym), id(id) { }

    Sym *sym;
    std::size_t id;

    pcx::data_ostringstream bytes;
};

#endif // FUNCTION_H
