#ifndef VM_H
#define VM_H

#include <pcx/datastream.h>

#include <cstdint>

namespace Vm
{

enum class Reg : std::uint8_t
{
    Pc,
    Bp,
    Sp,
    Dx,

    Invalid
};

const char *toString(Reg v);

enum class Op : std::uint8_t
{
    End,

    SetRI,

    SubRI,
    AddRI,

    CopyRR,
    CopyRA,
    CopyAR,
    CopyAI,
    CopyAA,

    PushR,
    PopR,

    Call,
    Ret,

    Svc,

    Invalid
};

const char *toString(Op v);

}

inline pcx::data_ostream &operator<<(pcx::data_ostream &os, Vm::Reg v){ return os << static_cast<std::uint8_t>(v); }
inline pcx::data_ostream &operator<<(pcx::data_ostream &os, Vm::Op v){ return os << static_cast<std::uint8_t>(v); }

#endif // VM_H
