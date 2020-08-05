#include "Sym.h"

#include "framework/Error.h"

#include "types/Type.h"

#include <pcx/str.h>
#include <pcx/join_str.h>

Sym::Sym(Type type, Location location, std::string name) : t(type), n(location), s(std::move(name)), ps(nullptr)
{
}

Sym::~Sym()
{
    for(auto s: cs)
    {
        delete s;
    }
}

Sym *Sym::add(Sym *sym)
{
    cs.push_back(sym);
    sym->ps = this;

    return sym;
}

Sym *Sym::child(const std::string &name)
{
    for(auto s: cs)
    {
        if(s->name() == name)
        {
            return s;
        }
    }

    return nullptr;
}

Sym *Sym::container()
{
    auto s = this;

    while(s)
    {
        switch(s->type())
        {
            case Type::Namespace:
            case Type::Class:
            case Type::Func: return s;

            default: s = s->parent();
        }
    }

    return nullptr;
}

void Sym::setProperty(const std::string &name, pcx::any value)
{
    pm[name] = value;
}

std::vector<std::string> Sym::names() const
{
    if(!ps)
    {
        return { name() };
    }

    std::vector<std::string> v;

    auto s = this;
    while(s->ps)
    {
        v.insert(v.begin(), s->s);
        s = s->ps;
    }

    return v;
}

std::string Sym::fullname() const
{
    return pcx::join_str(names(), ".");
}

std::string Sym::funcname() const
{
    return pcx::str(fullname(), assertProperty("type").to<::Type*>()->description());
}

pcx::any Sym::property(const std::string &name) const
{
    auto i = pm.find(name);
    if(i != pm.end())
    {
        return i->second;
    }

    return { };
}

pcx::any Sym::assertProperty(const std::string &name) const
{
    auto a = property(name);
    if(!a)
    {
        throw Error(location(), "Sym::assertProperty failed - ", name, " ", fullname());
    }

    return a;
}

const char *Sym::toString(Type v)
{
    static const char *s[] =
    {
        "namespace",
        "func",
        "class",
        "scope",
        "var",
        "invalid"
    };

    return s[static_cast<int>(v)];
}