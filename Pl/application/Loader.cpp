#include "Loader.h"

#include "framework/Error.h"

#include "application/Context.h"

#include <pcx/datastream.h>

void Loader::load(Context &c, const std::string &path)
{
    pcx::data_ifstream is(path);
    if(!is.is_open())
    {
        throw Error("unable to open - ", path);
    }

    auto u = new Object::Unit(path);
    c.units.push_back(u);

    is >> u->strings;

    auto count = is.get<std::size_t>();
    for(std::size_t i = 0; i < count; ++i)
    {
        auto e = new Object::Entity();
        u->entities.push_back(e);

        Object::load(is, *e);
    }

    std::ifstream ds(path + ".pmap");
    if(ds.is_open())
    {
        FileMap m;
        m.read(ds);

        for(std::size_t i = 0; i < m.size(); ++i)
        {
            auto &e = m[i];
            switch(e.type)
            {
                case 'V': c.dm.push_back(e); break;
                case 'F': c.cm.push_back(e); break;
            }
        }
    }
}
