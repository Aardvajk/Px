#ifndef TYPE_H
#define TYPE_H

#include "common/Primitives.h"

#include "scanner/Location.h"

#include "generics/GenericRef.h"

#include <pcx/optional.h>

#include <string>
#include <vector>
#include <iostream>

class Sym;

class Type
{
public:
    Type();

    std::string description() const;

    bool function() const;
    bool primitive() const;

    Primitive::Type primitiveType() const;

    pcx::optional<std::size_t> size() const;

    static Type primary(Sym *sym);
    static Type function(Type *returnType, const std::vector<Type*> &args = { });
    static Type generic(const GenericRef &gref);

    static bool exact(const Type *a, const Type *b);
    static bool exact(const std::vector<Type*> &a, const std::vector<Type*> &b);

    static std::size_t assertSize(Location location, const Type *type);

    Sym *sym;

    Type *returnType;
    std::vector<Type*> args;

    pcx::optional<GenericRef> gref;
};

#endif // TYPE_H
