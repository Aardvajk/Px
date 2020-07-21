#include "Linker.h"

#include "framework/Error.h"

#include "application/Context.h"

void Linker::link(Context &c)
{
    for(auto &u: c.units)
    {
        for(auto &e: u.entities)
        {
            for(auto n: e.links)
            {
                auto t = c.find(u.strings[n.id]);
                if(!t)
                {
                    throw Error("symbol not found - ", u.strings[n.id]);
                }

                auto addr = (t->type == 'F' ? c.ds.position() : 0) + t->offset;

                c.cs.writeAt(n.address + e.offset, reinterpret_cast<const char*>(&addr), sizeof(std::size_t));
            }
        }
    }
}
