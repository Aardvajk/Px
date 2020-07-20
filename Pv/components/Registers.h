#ifndef REGISTERS_H
#define REGISTERS_H

#include "common/OpCodes.h"

#include <cstring>

class Registers
{
public:
    Registers(){ }

    std::size_t &operator[](OpCode::Reg r){ return v[static_cast<int>(r)]; }

    std::size_t &pc(){ return operator[](OpCode::Reg::Pc); }
    std::size_t &sp(){ return operator[](OpCode::Reg::Sp); }
    std::size_t &bp(){ return operator[](OpCode::Reg::Bp); }
    std::size_t &dx(){ return operator[](OpCode::Reg::Dx); }

private:
    std::size_t v[4];
};

#endif // REGISTERS_H
