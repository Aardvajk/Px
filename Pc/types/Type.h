#ifndef TYPE_H
#define TYPE_H

#include "scanner/Location.h"

#include <pcx/optional.h>

#include <string>
#include <vector>

class Sym;

class Type
{
public:
    Type();

    std::string description() const;

    pcx::optional<std::size_t> size() const;
    std::size_t assertedSize(Location location) const;

    static Type primary(Sym *sym);
    static Type function(Type *returnType, const std::vector<Type*> &args = { });

    static bool compare(const Type *a, const Type *b);
    static bool compare(const std::vector<Type*> &a, const std::vector<Type*> &b);

    Sym *sym;

    Type *returnType;
    std::vector<Type*> args;
};

#endif // TYPE_H
