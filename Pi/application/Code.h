#ifndef CODE_H
#define CODE_H

#include <string>

namespace Code
{

enum class Type
{
    Push,
    Pop,

    Call,

    Load,
    Store,

    AllocS,

    Service,

    Invalid
};

const char *toString(Type v);
Type fromString(const std::string &v);

}

#endif // CODE_H
