#include "Machine.h"

#include "framework/Error.h"

#include "components/ByteReader.h"
#include "components/Stack.h"
#include "components/Vars.h"

Machine::Machine(const std::vector<char> &v, ServiceProc sp) : mm(1024), sp(sp)
{
    std::memcpy(mm(0), v.data(), v.size());

    rg.pc() = 0;
    rg.sp() = mm.size();
}

void Machine::execute()
{
    ByteReader rm(mm(0), rg.pc());
    Stack s(mm, rg);
    Vars v;

    while(true)
    {
        using namespace OpCode;

        rm(v.op);

        switch(v.op)
        {
            case Op::End: return;

            case Op::Svc: rm(v.i0); sp(v.i0, mm, rg); break;

            default: throw Error("invalid opcode");
        }
    }
}

void Machine::shutdown()
{
    if(rg.sp() != mm.size())
    {
    }

    std::cout << "shutdown ok\n";
}
