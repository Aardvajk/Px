#include "Machine.h"

#include "framework/Error.h"
#include "framework/ByteReader.h"

#include "components/Stack.h"
#include "components/Vars.h"

Machine::Machine(pcx::args &args, const std::vector<char> &v, ServiceProc sp) : args(args), mm(1024 * 10), sp(sp)
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

            case Op::SetRI: rm(v.r0); std::memcpy(&(rg[v.r0]), mm(rg.pc()), sizeof(std::size_t)); rg.pc() += sizeof(std::size_t); break;

            case Op::AddRI: rm(v.r0, v.s0); rg[v.r0] += v.s0; break;
            case Op::SubRI: rm(v.r0, v.s0); rg[v.r0] -= v.s0; break;

            case Op::CopyRR: rm(v.r0, v.r1); rg[v.r1] = rg[v.r0]; break;
            case Op::CopyRA: rm(v.r0, v.r1); std::memcpy(mm(rg[v.r1]), &(rg[v.r0]), sizeof(std::size_t)); break;
            case Op::CopyAR: rm(v.r0, v.r1); std::memcpy(&(rg[v.r1]), mm(rg[v.r0]), sizeof(std::size_t)); break;
            case Op::CopyAI: rm(v.r0, v.s0); std::memcpy(mm(rg[v.r0]), mm(rg.pc()), v.s0); rg.pc() += v.s0; break;
            case Op::CopyAA: rm(v.r0, v.r1, v.s0); std::memcpy(mm(rg[v.r1]), mm(rg[v.r0]), v.s0); break;

            case Op::PushR: rm(v.r0); s.push(rg[v.r0]); break;
            case Op::PopR: rm(v.r0); s.pop(rg[v.r0]); break;

            case Op::Call: rm(v.r0); s.push(rg.pc()); rg.pc() = rg[v.r0]; break;
            case Op::Ret: rm(v.s0); s.pop(rg.pc()); rg.sp() += v.s0; break;

            case Op::JmpNz: rm(v.s0); s.pop(v.c0); if(v.c0) rg.pc() += v.s0; break;

            case Op::Svc: rm(v.i0); sp(v.i0, mm, rg); break;

            default: throw Error("invalid opcode");
        }
    }
}

void Machine::shutdown()
{
    if(rg.sp() != mm.size())
    {
        throw Error("stack not empty at shutdown");
    }

    if(!args.contains("q"))
    {
        std::cout << "shutdown ok\n";
    }
}
