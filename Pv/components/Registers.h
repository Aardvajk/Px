#ifndef REGISTERS_H
#define REGISTERS_H

#include "common/Vm.h"

#include <cstring>

class Registers
{
public:
    Registers(){ }

    std::size_t &operator[](Vm::Reg r){ return v[static_cast<int>(r)]; }

    std::size_t &pc(){ return operator[](Vm::Reg::Pc); }
    std::size_t &sp(){ return operator[](Vm::Reg::Sp); }
    std::size_t &bp(){ return operator[](Vm::Reg::Bp); }
    std::size_t &dx(){ return operator[](Vm::Reg::Dx); }

private:
    std::size_t v[4];
};

#endif // REGISTERS_H
