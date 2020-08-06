#include "Loader.h"

#include "framework/Error.h"

#include "application/Context.h"

#include <pcx/datastream.h>
#include <pcx/scoped_ptr.h>

#include <unordered_set>

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

    std::unordered_set<std::string> ignored;

    auto count = is.get<std::size_t>();
    for(std::size_t i = 0; i < count; ++i)
    {
        pcx::scoped_ptr<Object::Entity> e = new Object::Entity();
        Object::load(is, *e);

        bool ignore = false;

        auto name = u->strings[e->id];
        if(c.find(name))
        {
            if(e->flags & Object::Entity::Flag::Autogen)
            {
                ignore = true;
                ignored.insert(name);
            }
            else
            {
                throw Error("duplicate symbol - ", name);
            }
        }

        if(!ignore)
        {
            u->entities.push_back(e.release());
        }
    }

    std::ifstream ds(path + ".pmap");
    if(ds.is_open())
    {
        FileMap m;
        m.read(ds);

        for(std::size_t i = 0; i < m.size(); ++i)
        {
            auto &e = m[i];

            if(ignored.find(e.name) == ignored.end())
            {
                switch(e.type)
                {
                    case 'V': c.dm.push_back(e); break;
                    case 'F': c.cm.push_back(e); break;
                }
            }
        }
    }
}
