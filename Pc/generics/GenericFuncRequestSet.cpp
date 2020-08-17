#include "GenericFuncRequestSet.h"

#include "generics/Generics.h"

GenericFuncRequestSet::GenericFuncRequestSet()
{
}

void GenericFuncRequestSet::insert(const GenericFuncRequest &request)
{
    auto s = Generics::funcname(request.sym, request.types);
    if(sc.find(s) == sc.end())
    {
        v.push_back(request);
        sc.insert(s);
    }
}

void GenericFuncRequestSet::clearPending()
{
    v.clear();
}
