#ifndef TYPECACHE_H
#define TYPECACHE_H

#include "types/Type.h"

#include <pcx/ptr_vector.h>

#include <iostream>

class Context;

class TypeCache
{
public:
    TypeCache(Context &c);

    Type *insert(const Type &type);

    Type *nullType();
    Type *intType();

    void print(std::ostream &os);

private:
    pcx::ptr_vector<Type> v;
};

#endif // TYPECACHE_H
