#include "Sym.h"

#include "framework/Error.h"

#include "types/Type.h"

#include <pcx/str.h>
#include <pcx/join_str.h>

Sym::Sym(Type type, Location location, std::string name) : t(type), n(location), s(name), ps(nullptr)
{
}

Sym::~Sym()
{
    for(auto s: cs)
    {
        delete s;
    }
}

Sym *Sym::add(Sym *s)
{
    s->ps = this;
    cs.push_back(s);

    return s;
}

void Sym::setProperty(const std::string &name, pcx::any value)
{
    pm[name] = value;
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
    return pcx::str(fullname(), assertedProperty("type").to<::Type*>()->description());
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

pcx::any Sym::assertedProperty(const std::string &name) const
{
    auto a = property(name);
    if(!a)
    {
        throw Error(location(), "Sym::assertProperty failed - ", fullname());
    }

    return a;
}

const char *Sym::toString(Type v)
{
    static const char *s[] = { "namespace", "func", "class", "var", "scope", "(invalid)" };
    return s[static_cast<int>(v)];
}
