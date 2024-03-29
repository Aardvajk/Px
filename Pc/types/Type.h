#ifndef TYPE_H
#define TYPE_H

#include "scanner/Location.h"

#include <pcx/optional.h>

#include <string>
#include <vector>
#include <iostream>

class Context;
class Sym;

class Type
{
public:
    Type();

    std::string description() const;

    bool primitive() const;

    pcx::optional<std::size_t> size() const;
    std::size_t assertedSize(Location location) const;

    static Type primary(Sym *sym);
    static Type function(Type *returnType, const std::vector<Type*> &args = { });

    Type addPointer() const;
    Type removePointer() const;
    Type removeRef() const;

    static bool compare(const Type *a, const Type *b);
    static bool compare(const std::vector<Type*> &a, const std::vector<Type*> &b);

    static bool exact(const Type *a, const Type *b);

    static bool compareNonTemplates(Context &c, const std::vector<Type*> &a, const std::vector<Type*> &b);

    bool ref;
    std::size_t ptr;

    Sym *sym;

    Type *returnType;
    std::vector<Type*> args;

    bool method;
};

inline std::ostream &operator<<(std::ostream &os, const Type *t){ return os << t->description(); }

#endif // TYPE_H
