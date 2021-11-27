#ifndef OPCODES_H
#define OPCODES_H

#include <pcx/datastream.h>

#include <cstdint>

namespace OpCode
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

    JmpNz,

    Svc,
    Invalid
};

const char *toString(Op v);

}

inline pcx::data_ostream &operator<<(pcx::data_ostream &os, OpCode::Reg v){ return os << static_cast<std::uint8_t>(v); }
inline pcx::data_ostream &operator<<(pcx::data_ostream &os, OpCode::Op v){ return os << static_cast<std::uint8_t>(v); }

#endif // OPCODES_H
