#include "Primitives.h"

namespace
{

const char *s[] =
{
    "char",
    "int",
    ""
};

}

const char *Primitive::toString(Type v)
{
    return s[static_cast<int>(v)];
}

Primitive::Type Primitive::fromString(const std::string &v)
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
