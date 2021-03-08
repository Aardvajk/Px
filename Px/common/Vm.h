#ifndef VM_H
#define VM_H

#include <cstdint>

namespace Vm
{

enum class Reg
{
    Invalid
};

const char *toString(Reg v);

enum class Op
{
    Invalid
};

const char *toString(Op v);

}

#endif // VM_H
