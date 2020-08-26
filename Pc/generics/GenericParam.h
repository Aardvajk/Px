#ifndef GENERICPARAM_H
#define GENERICPARAM_H

#include <string>

class Type;

class GenericParam
{
public:
    GenericParam(std::string name, Type *type) : name(std::move(name)), type(type) { }

    std::string name;
    Type *type;
};

#endif // GENERICPARAM_H
