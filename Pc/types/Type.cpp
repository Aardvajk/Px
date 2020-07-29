#include "Type.h"

#include "syms/Sym.h"

#include <pcx/join_str.h>

#include <sstream>

namespace
{

std::string describeType(const Type *t)
{
    std::ostringstream os;

    if(t->sym)
    {
        os << t->sym->fullname();
    }
    else if(t->returnType)
    {
        os << "(" << pcx::join_str(t->args, ", ", [](const Type *t){ return t->description(); }) << "):" << t->returnType->description();
    }

    return os.str();
}

bool compareTypes(const Type *a, const Type *b)
{
    if(!a && !b)
    {
        return true;
    }

    if(!a || !b)
    {
        return false;
    }

    if(a->sym != b->sym)
    {
        return false;
    }

    if(a->args.size() != b->args.size())
    {
        return false;
    }

    if(!compareTypes(a->returnType, b->returnType))
    {
        return false;
    }

    for(std::size_t i = 0; i < a->args.size(); ++i)
    {
        if(!compareTypes(a->args[i], b->args[i]))
        {
            return false;
        }
    }

    return true;
}

}

Type::Type() : sym(nullptr), returnType(nullptr)
{
}

std::string Type::description() const
{
    return describeType(this);
}

Type Type::primary(Sym *sym)
{
    Type t;

    t.sym = sym;

    return t;
}

Type Type::function(Type *returnType, const std::vector<Type*> &args)
{
    Type t;

    t.returnType = returnType;
    t.args = args;

    return t;
}

bool Type::exact(const Type *a, const Type *b)
{
    return compareTypes(a, b);
}
