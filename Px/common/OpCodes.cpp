#include "OpCodes.h"

const char *OpCode::toString(Reg v)
{
    static const char *s[] =
    {
    };

    return s[static_cast<int>(v)];
}

const char *OpCode::toString(Op v)
{
    static const char *s[] =
    {
    };

    return s[static_cast<int>(v)];
}
