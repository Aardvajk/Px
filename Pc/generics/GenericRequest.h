#ifndef GENERICREQUEST_H
#define GENERICREQUEST_H

#include <vector>

class Sym;
class Type;

class GenericRequest
{
public:
    GenericRequest(Sym *sym, std::vector<Type*> types) : sym(sym), types(std::move(types)) { }

    Sym *sym;
    std::vector<Type*> types;
};

#endif // GENERICREQUEST_H
