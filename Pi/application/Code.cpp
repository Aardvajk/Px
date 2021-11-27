#include "Code.h"

namespace
{

const char *s[] =
{
    "push",
    "pop",

    "call",
    "jump",

    "load",
    "store",

    "allocs",
    "incrs",

    "service",

    ""
};

}

const char *Code::toString(Type v)
{
    return s[static_cast<int>(v)];
}

Code::Type Code::fromString(const std::string &v)
{
    for(int i = 0; s[i][0]; ++i)
    {
        if(s[i] == v)
        {
            return static_cast<Type>(i);
        }
    }

    return Type::Invalid;
}
