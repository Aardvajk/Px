#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <vector>

class Sym;

class Type
{
public:
    Type();

    std::string description() const;

    static Type primary(Sym *sym);
    static Type function(Type *returnType, const std::vector<Type*> &args = { });

    static bool exact(const Type *a, const Type *b);
    static bool exact(const std::vector<Type*> &a, const std::vector<Type*> &b);

    Sym *sym;

    Type *returnType;
    std::vector<Type*> args;
};

#endif // TYPE_H
