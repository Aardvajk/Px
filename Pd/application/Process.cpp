#include "Process.h"

#include "framework/Console.h"
#include "framework/ByteReader.h"

#include "common/Object.h"

#include "application/Context.h"
#include "application/Disassembler.h"

#include <pcx/indexed_range.h>

namespace
{

void function(Context &c, Object::Unit &u, std::size_t index, Object::Entity &e, std::ostream &os)
{
    Disassembler::disassemble(c, index, e.data.data(), e.data.size(), os);
}

}

void Process::unit(Context &c, const std::string &path, pcx::data_istream &is, std::ostream &os)
{
    Object::Unit u(path);

    is >> u.strings;

    auto n = is.get<std::size_t>();
    for(std::size_t i = 0; i < n; ++i)
    {
        u.entities.push_back(new Object::Entity());
        Object::load(is, u.entities.back());
    }

    os << banner(path, " stringtable");

    auto pw = padw(u.strings.size());
    for(auto s: pcx::indexed_range(u.strings))
    {
        std::cout << pad(s.index, pw) << ": " << s.value << "\n";
    }

    for(auto e: pcx::indexed_range(u.entities))
    {
        os << banner(e.value.type, " ", u.strings[e.value.id]);

        switch(e.value.type)
        {
            case 'F': function(c, u, e.index, e.value, os); break;
        }
    }
}

void Process::executable(Context &c, const std::string &path, pcx::data_istream &is, std::ostream &os)
{
    auto v = is.all();

    os << banner(path);

    if(c.dm.size())
    {
        std::size_t pc = 0;

        for(std::size_t i = 0; i < c.dm.size(); ++i)
        {
            auto &e = c.dm[i];

            if(!e.name.empty())
            {
                os << banner(e.type, " ", e.name);
            }

            if(e.type == 'V')
            {
            }
            else if(e.type == 'F')
            {
                Disassembler::disassemble(c, i, v.data() + pc, e.size, os);
            }

            pc += e.size;
        }
    }
    else
    {
        Disassembler::disassemble(c, 0, v.data(), v.size(), os);
    }
}

