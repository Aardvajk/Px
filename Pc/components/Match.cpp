#include "Match.h"

#include "types/Type.h"

namespace
{

void process(Context &c, const Type *expected, Type *candidate, Match &m)
{
    if(Type::exact(expected, candidate))
    {
        ++m.exacts;
    }
}

}

Match::Match() : exacts(0), valid(false)
{
}

Match Match::create(Context &c, const Type *expected, const Type *candidate)
{
    Match m;
    if(expected->args.size() != candidate->args.size())
    {
        return m;
    }

    for(std::size_t i = 0; i < expected->args.size(); ++i)
    {
        process(c, expected->args[i], candidate->args[i], m);
    }

    m.valid = true;
    return m;
}

