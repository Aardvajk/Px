#include "Type.h"

#include "framework/Error.h"

#include "syms/Sym.h"

#include <pcx/join_str.h>

#include <sstream>

namespace
{

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

    if(!compareTypes(a->returnType, b->returnType))
    {
        return false;
    }

    if(a->args.size() != b->args.size())
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

std::string describeType(const Type *t)
{
    std::ostringstream os;

    if(t->sym)
    {
        os << t->sym->fullname();
    }
    else if(t->returnType)
    {
        os << "(" << pcx::join_str(t->args, ",", [](const Type *t){ return t->description(); }) << "):" << t->returnType->description();
    }

    return os.str();
}

}

Type::Type() : sym(nullptr), returnType(nullptr)
{
}

std::string Type::description() const
{
    return describeType(this);
}

pcx::optional<std::size_t> Type::size() const
{
    if(returnType)
    {
        return sizeof(std::size_t);
    }

    if(sym)
    {
        if(auto sz = sym->property("size"))
        {
            return sz.to<std::size_t>();
        }
    }

    return { };
}

std::size_t Type::assertedSize(Location location) const
{
    auto s = size();

    if(!s)
    {
        throw Error(location, "use of incomplete type - ", description());
    }

    return *s;
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

bool Type::compare(const Type *a, const Type *b)
{
    return compareTypes(a, b);
}

bool Type::compare(const std::vector<Type*> &a, const std::vector<Type*> &b)
{
    if(a.size() != b.size())
    {
        return false;
    }

    for(std::size_t i = 0; i < a.size(); ++i)
    {
        if(!compareTypes(a[i], b[i]))
        {
            return false;
        }
    }

    return true;
}
