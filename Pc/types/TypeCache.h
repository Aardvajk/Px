#ifndef TYPECACHE_H
#define TYPECACHE_H

#include "common/Primitives.h"

#include "types/Type.h"

#include <pcx/ptr_vector.h>
#include <pcx/scoped_ptr.h>

#include <iostream>
#include <unordered_map>

class Context;
class Sym;

class TypeCache
{
public:
    TypeCache(Context &c);

    Type *insert(const Type &type);

    Type *nullType();
    Type *charType();
    Type *intType();

    bool isUnknownTemplateType(const Type *type) const;

    Type *unknownTemplateType();

    pcx::ptr_vector<Type>::iterator begin(){ return v.begin(); }
    pcx::ptr_vector<Type>::iterator end(){ return v.end(); }

private:
    void createPrimitive(Context &c, const std::string &name, Sym *container, Primitive::Type type, std::size_t size);

    pcx::ptr_vector<Type> v;
    pcx::scoped_ptr<Sym> unknownTemplateSym;
};

#endif // TYPECACHE_H
