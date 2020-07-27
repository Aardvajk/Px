#include "Sym.h"

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

const char *Sym::toString(Type v)
{
    static const char *s[] =
    {
        "namespace",
        "invalid"
    };

    return s[static_cast<int>(v)];
}
