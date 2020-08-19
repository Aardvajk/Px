#include "Match.h"

#include "types/Type.h"

#include "generics/Generics.h"

namespace
{

void process(Context &c, const Type *expected, Type *candidate, const std::vector<Type*> &generics, Match &m)
{
    if(Type::exact(expected, candidate))
    {
        ++m.exacts;
    }
    else if(candidate->gref)
    {
        ++m.generics;
    }
}

}

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
        process(c, expected->args[i], candidate->args[i], generics, m);
    }

    m.valid = true;
    return m;
}

