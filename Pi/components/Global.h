#ifndef GLOBAL_H
#define GLOBAL_H

#include <cstddef>

class Sym;

class Global
{
public:
    Global(Sym *sym, std::size_t id) : sym(sym), id(id) { }

    Sym *sym;
    std::size_t id;
};

#endif // GLOBAL_H
