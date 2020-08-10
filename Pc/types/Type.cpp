#include "Type.h"

#include "framework/Error.h"

#include "syms/Sym.h"

#include <pcx/join_str.h>

#include <sstream>

namespace
{

std::string describeType(const Type *t, const std::vector<Type*> &generics);

class Functor
{
public:
    Functor(const std::vector<Type*> &v) : v(v) { }

    std::string operator()(const Type *t) const { return describeType(t, v); }

private:
    const std::vector<Type*> &v;
};

std::string describeType(const Type *t, const std::vector<Type*> &generics)
{
    std::ostringstream os;

    if(t->sym)
    {
        os << t->sym->fullname();
    }
    else if(t->returnType)
    {
        os << "(" << pcx::join_str(t->args, ",", Functor(generics)) << "):" << t->returnType->convertedDescription(generics);
    }
    else if(t->gref)
    {
        if(t->gref->index < generics.size())
        {
            os << describeType(generics[t->gref->index], { });
        }
        else
        {
            os << "<" << t->gref->index << ">";
        }
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

    if(a->gref != b->gref)
    {
        return false;
    }

    return true;
}

}

Type::Type() : sym(nullptr), returnType(nullptr)
{
}

std::string Type::description() const
{
    return describeType(this, { });
}

std::string Type::convertedDescription(const std::vector<Type*> &types) const
{
    return describeType(this, types);
}

bool Type::function() const
{
    return returnType;
}

bool Type::primitive() const
{
    return function() || (sym && sym->property("primitive"));
}

Primitive::Type Type::primitiveType() const
{
    if(function())
    {
        return Primitive::Type::ULong;
    }

    if(sym)
    {
        if(auto p = sym->property("primitive"))
        {
            return p.to<Primitive::Type>();
        }
    }

    return Primitive::Type::Invalid;
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

Type Type::generic(const GenericRef &ref)
{
    Type t;

    t.gref = ref;

    return t;
}

bool Type::exact(const Type *a, const Type *b)
{
    return compareTypes(a, b);
}

bool Type::exact(const std::vector<Type*> &a, const std::vector<Type*> &b)
{
    if(a.size() != b.size())
    {
        return false;
    }

    for(std::size_t i = 0; i < a.size(); ++i)
    {
        if(!exact(a[i], b[i]))
        {
            return false;
        }
    }

    return true;
}

std::size_t Type::assertSize(Location location, const Type *type)
{
    if(!type)
    {
        throw Error(location, "Type::assertSize null type");
    }

    if(auto s = type->size())
    {
        return *s;
    }

    throw Error(location, "incomplete type - ", type->description());
}
