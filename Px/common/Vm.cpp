#include "Vm.h"

const char *Vm::toString(Reg v)
{
    static const char *s[] =
    {
        "(invalid)"
    };

    return s[static_cast<int>(v)];
}

const char *Vm::toString(Op v)
{
    static const char *s[] =
    {
        "(invalid)"
    };

    return s[static_cast<int>(v)];
}
