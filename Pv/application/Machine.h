#ifndef MACHINE_H
#define MACHINE_H

#include "components/Memory.h"
#include "components/Registers.h"

#include <pcx/args.h>

class Machine
{
public:
    using ServiceProc = void(*)(int, Memory&, Registers&);

    Machine(pcx::args &args, const std::vector<char> &v, ServiceProc sp);

    void execute();
    void shutdown();

private:
    pcx::args &args;

    Memory mm;
    Registers rg;
    ServiceProc sp;
};

#endif // MACHINE_H
