#include "GenericRequestSet.h"

#include "generics/Generics.h"

GenericRequestSet::GenericRequestSet()
{
}

void GenericRequestSet::insert(const GenericRequest &request)
{
    auto s = Generics::funcname(request.sym, request.types);
    if(sc.find(s) == sc.end())
    {
        v.push_back(request);
        sc.insert(s);
    }
}

void GenericRequestSet::clearPending()
{
    v.clear();
}
