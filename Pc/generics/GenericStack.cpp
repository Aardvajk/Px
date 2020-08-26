#include "GenericStack.h"

#include "nodes/Node.h"

#include "visitors/NameVisitors.h"

GenericStack::GenericStack()
{
}

void GenericStack::push_back(const GenericParamList &g)
{
    v.push_back(g);
}

void GenericStack::pop_back()
{
    v.pop_back();
}

pcx::optional<GenericRef> GenericStack::typeRef(Node *name) const
{
    if(Visitor::query<NameVisitors::IsNameSimple, bool>(name) && !v.empty())
    {
        auto n = Visitor::query<NameVisitors::TrailingId, std::string>(name);

        std::size_t index = v.size() - 1;
        std::size_t depth = 0;

        while(true)
        {
            auto &gv = v[index];

            for(std::size_t i = 0; i < gv.size(); ++i)
            {
                if(gv[i].name == n)
                {
                    return GenericRef(i, depth);
                }
            }

            if(index-- == 0) break;
            depth++;
        }
    }

    return { };
}
