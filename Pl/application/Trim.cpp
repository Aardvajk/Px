#include "Trim.h"

#include "application/Context.h"

void Trim::generateRefs(Context &c, Object::Entity *e)
{
    for(auto n: e->links)
    {
        auto r = c.units[e->unit].strings[n.id];
        c.refs.insert(r);

        auto re = c.find(r);
        if(re)
        {
            generateRefs(c, re);
        }
    }
}

void Trim::updateFileMap(Context &c, FileMap &dm)
{
    FileMap rm;
    std::unordered_set<std::string> done;

    for(std::size_t i = 0; i < dm.size(); ++i)
    {
        if(dm[i].name.empty() || c.refs.find(dm[i].name) != c.refs.end())
        {
            if(done.find(dm[i].name) == done.end())
            {
                rm.push_back(dm[i]);
                done.insert(dm[i].name);
            }
        }
    }

    dm = rm;
}

