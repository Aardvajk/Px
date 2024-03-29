#include "Process.h"

#include "framework/ByteReader.h"

#include "common/Object.h"

#include "application/Context.h"
#include "application/Disassembler.h"

#include <pcx/format.h>
#include <pcx/indexed_range.h>

void Process::unit(Context &c, const std::string &path, pcx::data_istream &is, std::ostream &os)
{
    Object::Unit u(path);

    is >> u.strings;

    auto n = is.get<std::size_t>();
    for(std::size_t i = 0; i < n; ++i)
    {
        u.entities.push_back(new Object::Entity());
        Object::load(is, 0, u.entities.back());
    }

    os << pcx::format::banner(path, " stringtable");

    auto pw = pcx::format::padw(u.strings.size());
    for(auto s: pcx::indexed_range(u.strings))
    {
        std::cout << pcx::format::pad(s.index, pw) << ": " << s.value << "\n";
    }

    for(auto e: pcx::indexed_range(u.entities))
    {
        os << pcx::format::banner(e.value.type, " ", static_cast<std::uint32_t>(e.value.flags), " ", u.strings[e.value.id]);

        switch(e.value.type)
        {
            case 'V': Disassembler::value(c, e.index, e.value.data.data(), e.value.data.size(), os); break;
            case 'F': Disassembler::function(c, e.index, e.value.data.data(), e.value.data.size(), os); break;
        }
    }
}

void Process::executable(Context &c, const std::string &path, pcx::data_istream &is, std::ostream &os)
{
    auto v = is.all();

    os << pcx::format::banner(path);

    if(c.dm.size())
    {
        std::size_t pc = 0;

        for(std::size_t i = 0; i < c.dm.size(); ++i)
        {
            auto &e = c.dm[i];

            if(!e.name.empty())
            {
                os << pcx::format::banner(e.type, " ", e.name);
            }

            switch(e.type)
            {
                case 'V': Disassembler::value(c, i, v.data() + pc, e.size, os); break;
                case 'F': Disassembler::function(c, i, v.data() + pc, e.size, os); break;
            }

            pc += e.size;
        }
    }
    else
    {
        Disassembler::function(c, 0, v.data(), v.size(), os);
    }
}

