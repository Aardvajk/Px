#include "Generator.h"

#include "application/Context.h"

void Generator::generate(Context &c)
{
    for(auto &u: c.units)
    {
        for(auto &e: u.entities)
        {
            if(!c.args.contains("trim") || c.refs.find(u.strings[e.id]) != c.refs.end())
            {
                auto &s = (e.type == 'V' ? c.ds : c.cs);

                if(!c.args.contains("q"))
                {
                    std::cout << "generating " << u.strings[e.id] << "\n";
                }

                e.offset = s.position();
                s.write(e.data.data(), e.data.size());
            }
        }
    }
}
