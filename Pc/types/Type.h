#ifndef TYPE_H
#define TYPE_H

#include "scanner/Location.h"

#include "generics/GenericRef.h"

#include <pcx/optional.h>

#include <string>
#include <vector>

class Sym;

class Type
{
public:
    Type();

    std::string description() const;
    std::string convertedDescription(const std::vector<Type*> &types) const;

    bool function() const { return returnType; }

    pcx::optional<std::size_t> size() const;

    static Type primary(Sym *sym);
    static Type function(Type *returnType, const std::vector<Type*> &args = { });
    static Type generic(const GenericRef &ref);

    static bool exact(const Type *a, const Type *b);
    static bool exact(const std::vector<Type*> &a, const std::vector<Type*> &b);

    static std::size_t assertSize(Location location, const Type *type);

    Sym *sym;

    Type *returnType;
    std::vector<Type*> args;

    pcx::optional<GenericRef> gref;
};

#endif // TYPE_H