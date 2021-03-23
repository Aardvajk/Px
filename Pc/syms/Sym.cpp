#include "Sym.h"

Sym::Sym()
{
}

const char *Sym::toString(Type v)
{
    static const char *s[] =
    {
        "namespace",
        "func",
        "class",
        "(invalid)"
    };

    return s[static_cast<int>(v)];
}
