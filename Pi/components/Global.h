#ifndef GLOBAL_H
#define GLOBAL_H

#include <pcx/datastream.h>

#include <cstddef>

class Sym;

class Global
{
public:
    Global(Sym *sym, std::size_t id) : sym(sym), id(id) { }

    Sym *sym;
    std::size_t id;

    pcx::data_ostringstream bytes;
};

#endif // GLOBAL_H
