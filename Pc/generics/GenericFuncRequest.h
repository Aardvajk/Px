#ifndef GENERICFUNCREQUEST_H
#define GENERICFUNCREQUEST_H

#include <vector>

class Sym;
class Type;

class GenericFuncRequest
{
public:
    GenericFuncRequest(Sym *sym, std::vector<Type*> types) : sym(sym), types(std::move(types)) { }

    Sym *sym;
    std::vector<Type*> types;
};

#endif // GENERICFUNCREQUEST_H
