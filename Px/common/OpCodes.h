#ifndef OPCODES_H
#define OPCODES_H

#include <cstdint>

namespace OpCode
{

enum class Reg : std::uint8_t
{
};

const char *toString(Reg v);

enum class Op : std::uint8_t
{
};

const char *toString(Op v);

}

#endif // OPCODES_H
