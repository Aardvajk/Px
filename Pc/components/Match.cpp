#include "Match.h"

#include "types/Type.h"

Match::Match() : exacts(0), generics(0)
{
}

Match Match::create(const Type *expected, const Type *candidate)
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
    }

    return m;
}

