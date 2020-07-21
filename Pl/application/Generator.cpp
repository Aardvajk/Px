#include "Generator.h"

#include "application/Context.h"

void Generator::generate(Context &c)
{
    for(auto &u: c.units)
    {
        for(auto &e: u.entities)
        {
            auto &s = (e.type == 'V' ? c.ds : c.cs);

            e.offset = s.position();
            s.write(e.data.data(), e.data.size());
        }
    }
}
