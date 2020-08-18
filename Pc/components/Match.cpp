#include "Match.h"

#include "types/Type.h"

#include "generics/Generics.h"

Match::Match() : exacts(0), generics(0), valid(false)
{
}

Match Match::create(Context &c, const Type *expected, const Type *candidate, const std::vector<Type*> &generics)
{
    Match m;
    if(expected->args.size() != candidate->args.size())
    {
        return m;
    }

    for(std::size_t i = 0; i < expected->args.size(); ++i)
    {
        if(Type::exact(expected->args[i], candidate->args[i]))
        {
            ++m.exacts;
        }
        else if(candidate->args[i]->gref)
        {
            ++m.generics;
        }
        else if(!generics.empty())
        {
            auto temp = Generics::updateTypeFromTypes(c, candidate->args[i], generics);
            if(Type::exact(expected->args[i], temp))
            {
                ++m.generics;
            }
        }
    }

    m.valid = true;
    return m;
}

