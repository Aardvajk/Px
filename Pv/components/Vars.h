#ifndef VARS_H
#define VARS_H

#include "common/Vm.h"

#include <cstddef>

class Vars
{
public:
    Vm::Op op;
    Vm::Reg r0, r1;
    std::size_t s0;
    int i0;
};

#endif // VARS_H
