#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <string>

namespace Primitive
{

enum class Type
{
    Char,
    Int,
    Invalid
};

const char *toString(Type v);
Type fromString(const std::string &v);

}

#endif // PRIMITIVES_H
