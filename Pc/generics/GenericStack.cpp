#include "GenericStack.h"

#include "application/Context.h"

#include "visitors/NameVisitors.h"

#include "nodes/Node.h"

#include <pcx/indexed_range.h>

#include <pcx/str.h>
#include <pcx/join_str.h>

pcx::optional<GenericRef> GenericStack::typeRef(Node *name) const
{
    if(Visitor::query<NameVisitors::IsNameSimple, bool>(name))
    {
        auto n = Visitor::query<NameVisitors::TrailingId, std::string>(name);

        for(auto &gs: v)
        {
            for(auto g: pcx::indexed_range(gs))
            {
                if(g.value.name == n)
                {
                    return GenericRef(g.index);
                }
            }
        }
    }

    return { };
}

Type *GenericStack::updateType(Context &c, Type *type) const
{
    auto copy = *type;

    if(copy.gref && !v.empty() && copy.gref->index < v.back().size())
    {
        if(auto t = v.back()[copy.gref->index].type)
        {
            copy = *t;
        }
    }

    for(auto &g: copy.generics)
    {
        g = updateType(c, g);
    }

    if(copy.sym && !copy.generics.empty())
    {
        auto name = pcx::str(copy.sym->name(), "<", pcx::join_str(copy.generics, ",", [](const Type *t){ return t->description(); }), ">");

        if(auto s = copy.sym->parent()->child(name))
        {
            copy = *c.types.insert(Type::primary(s));
        }
    }

    return c.types.insert(copy);
}
