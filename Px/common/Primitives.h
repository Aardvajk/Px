#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <pcx/datastream.h>

#include <string>

namespace Primitive
{

enum class Type : std::uint8_t
{
    Null,
    Char,
    Int,
    Invalid
};

const char *toString(Type v);
Type fromString(const std::string &v);

}

inline pcx::data_ostream &operator<<(pcx::data_ostream &os, Primitive::Type v){ return os << static_cast<std::uint8_t>(v); }

#endif // PRIMITIVES_H
