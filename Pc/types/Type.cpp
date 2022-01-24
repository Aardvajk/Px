#include "Type.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "syms/Sym.h"

#include <pcx/join_str.h>

#include <sstream>

namespace
{

bool compareTypes(const Type *a, const Type *b, bool exact)
{
    if(!a && !b)
    {
        return true;
    }

    if(!a || !b)
    {
        return false;
    }

    if(a->ptr != b->ptr)
    {
        return false;
    }

    if(a->sym != b->sym)
    {
        return false;
    }

    if(!compareTypes(a->returnType, b->returnType, exact))
    {
        return false;
    }

    if(a->args.size() != b->args.size())
    {
        return false;
    }

    for(std::size_t i = 0; i < a->args.size(); ++i)
    {
        if(!compareTypes(a->args[i], b->args[i], exact))
        {
            return false;
        }
    }

    if(a->method != b->method)
    {
        return false;
    }

    if(exact)
    {
        if(a->ref != b->ref)
        {
            return false;
        }
    }

    return true;
}

std::string describeType(const Type *t)
{
    std::ostringstream os;

    if(t->ref)
    {
        os << "ref ";
    }

    for(std::size_t i = 0; i < t->ptr; ++i)
    {
        os << "ptr ";
    }

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

Type::Type() : ref(false), ptr(0), sym(nullptr), returnType(nullptr), method(false)
{
}

std::string Type::description() const
{
    return describeType(this);
}

bool Type::primitive() const
{
    if(ptr || returnType)
    {
        return true;
    }

    if(sym)
    {
        return sym->property("primitive");
    }

    return false;
}

pcx::optional<std::size_t> Type::size() const
{
    if(ptr || returnType)
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

Type Type::addPointer() const
{
    auto t = *this;
    ++t.ptr;

    return t;
}

Type Type::removePointer() const
{
    auto t = *this;
    --t.ptr;

    return t;
}

bool Type::compare(const Type *a, const Type *b)
{
    return compareTypes(a, b, false);
}

bool Type::compare(const std::vector<Type*> &a, const std::vector<Type*> &b)
{
    if(a.size() != b.size())
    {
        return false;
    }

    for(std::size_t i = 0; i < a.size(); ++i)
    {
        if(!compareTypes(a[i], b[i], false))
        {
            return false;
        }
    }

    return true;
}

bool Type::exact(const Type *a, const Type *b)
{
    return compareTypes(a, b, true);
}

bool Type::compareNonTemplates(Context &c, const std::vector<Type*> &a, const std::vector<Type*> &b)
{
    if(a.size() != b.size())
    {
        return false;
    }

    for(std::size_t i = 0; i < a.size(); ++i)
    {
        if(!c.types.isUnknownTemplateType(a[i]) && !c.types.isUnknownTemplateType(b[i]))
        {
            if(!compareTypes(a[i], b[i], false))
            {
                return false;
            }
        }
    }

    return true;
}
