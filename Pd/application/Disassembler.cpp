#include "Disassembler.h"

#include "common/OpCodes.h"

#include "framework/ByteReader.h"
#include "framework/Console.h"
#include "framework/Error.h"

#include "application/Context.h"

#include <pcx/join_str.h>
#include <pcx/lexical_cast.h>

namespace
{

void map(Context &c, std::size_t index, std::size_t &map, std::size_t pc, std::ostream &os)
{
    if(index < c.dm.size())
    {
        auto &e = c.dm[index].comments;

        while(map < e.size() && e[map].address <= pc)
        {
            auto s = e[map++].comment;

            if(s.length() && s[0] == '-')
            {
                os << custom_banner('-', s.substr(1));
            }
            else
            {
                os << s << "\n";
            }
        }
    }
}

std::string bytes(ByteReader &rm, std::size_t n)
{
    std::vector<char> v;
    for(std::size_t i = 0; i < n; ++i)
    {
        v.push_back(rm.get<char>());
    }

    return pcx::join_str(v, ",", [](char c){ return pcx::lexical_cast<std::string>(int(c)); });
}

}

void Disassembler::value(Context &c, std::size_t index, const char *data, std::size_t size, std::ostream &os)
{
    std::size_t pc = 0;
    std::size_t mapIndex = 0;
    ByteReader rm(data, pc);

    map(c, index, mapIndex, pc, os);

    os << bytes(rm, size) << "\n";
}

void Disassembler::function(Context &c, std::size_t index, const char *data, std::size_t size, std::ostream &os)
{
    std::size_t pc = 0;
    std::size_t mapIndex = 0;
    ByteReader rm(data, pc);

    auto pcw = padw(size);

    while(pc < size)
    {
        using namespace OpCode;

        Op op;
        Reg r0, r1;
        std::size_t s0;
        int i0;

        map(c, index, mapIndex, pc, os);

        os << pad(pc, pcw) << ": ";

        rm(op);
        os << pad(toString(op), 7) << " ";

        switch(op)
        {
            case Op::End: break;

            case Op::SetRI: rm(r0); os << toString(r0) << " " << bytes(rm, 8); break;

            case Op::AddRI: rm(r0, s0); os << toString(r0) << " " << s0; break;
            case Op::SubRI: rm(r0, s0); os << toString(r0) << " " << s0;  break;

            case Op::CopyRR: rm(r0, r1); os << toString(r0) << " " << toString(r1); break;
            case Op::CopyRA: rm(r0, r1); os << toString(r0) << " " << toString(r1); break;
            case Op::CopyAR: rm(r0, r1); os << toString(r0) << " " << toString(r1); break;
            case Op::CopyAI: rm(r0, s0); os << toString(r0) << " " << s0 << " " << bytes(rm, s0); break;
            case Op::CopyAA: rm(r0, r1, s0); os << toString(r0) << " " << toString(r1) << " " << s0; break;

            case Op::PushR: rm(r0); os << toString(r0); break;
            case Op::PopR: rm(r0); os << toString(r0); break;

            case Op::Call: rm(r0); os << toString(r0); break;
            case Op::Ret: rm(s0); os << s0; break;

            case Op::JmpNz: rm(s0); os << s0; break;

            case Op::Svc: rm(i0); os << i0; break;

            default: throw Error("invalid opcode - ", static_cast<int>(op));
        }

        os << "\n";
    }

    map(c, index, mapIndex, pc, os);
}
