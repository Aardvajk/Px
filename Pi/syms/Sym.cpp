#include "Sym.h"

Sym::Sym(Type type, std::string name) : type(type), name(std::move(name))
{
}

pcx::any Sym::property(const std::string &name) const
{
    auto p = properties.find(name);
    if(p != properties.end())
    {
        return p->second;
    }

    return { };
}

const char *Sym::toString(Type v)
{
    static const char *s[] = { "global", "func", "var", "arg", "label", "(invalid)" };
    return s[static_cast<int>(v)];
}
