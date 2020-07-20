#ifndef VARS_H
#define VARS_H

#include "common/OpCodes.h"

#include <cstddef>

class Vars
{
public:
    OpCode::Op op;
    OpCode::Reg r0, r1;
    std::size_t s0, s1;
    int i0, i1;
    char c0;
};

#endif // VARS_H
