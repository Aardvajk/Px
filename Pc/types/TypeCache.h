#ifndef TYPECACHE_H
#define TYPECACHE_H

#include "types/Type.h"

#include <pcx/ptr_vector.h>

class TypeCache
{
public:
    TypeCache();

    Type *insert(const Type &type);

    Type *nullType();
    Type *charType();
    Type *intType();
    Type *boolType();

private:
    pcx::ptr_vector<Type> v;
};

#endif // TYPECACHE_H
