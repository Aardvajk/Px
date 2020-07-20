#include "OpCodes.h"

const char *OpCode::toString(Reg v)
{
    static const char *s[] =
    {
        "pc",
        "bp",
        "sp",
        "dx",
        "(invalid)"
    };

    return s[static_cast<int>(v)];
}

const char *OpCode::toString(Op v)
{
    static const char *s[] =
    {
        "end",
        "svc",
        "(invalid)"
    };

    return s[static_cast<int>(v)];
}
