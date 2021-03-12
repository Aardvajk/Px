#include "Vm.h"

const char *Vm::toString(Reg v)
{
    static const char *s[] =
    {
        "pc",
        "bp",
        "sp",
        "dx",

        "(invalid)"
    };

    return s[static_cast<int>(v)];
}

const char *Vm::toString(Op v)
{
    static const char *s[] =
    {
        "end",

        "setri",

        "subri",
        "addri",

        "copyrr",
        "copyra",
        "copyar",
        "copyai",
        "copyaa",

        "pushr",
        "popr",

        "call",
        "ret",

        "(invalid)"
    };

    return s[static_cast<int>(v)];
}
