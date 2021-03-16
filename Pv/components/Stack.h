#ifndef STACK_H
#define STACK_H

#include "components/Memory.h"

class Stack
{
public:
    Stack(Memory &mm, std::size_t &sp) : mm(mm), sp(sp) { }

    template<typename T> void push(T v){ sp -= sizeof(T); std::memcpy(mm(sp), &v, sizeof(T)); }
    template<typename T> void pop(T &v){ std::memcpy(&v, mm(sp), sizeof(T)); sp += sizeof(T); }

private:
    Memory &mm;
    std::size_t &sp;
};

#endif // STACK_H
