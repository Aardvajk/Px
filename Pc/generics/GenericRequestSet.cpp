#include "GenericRequestSet.h"

GenericRequestSet::GenericRequestSet()
{
}

void GenericRequestSet::insert(const GenericRequest &request)
{
    v.push_back(request);
}

void GenericRequestSet::clearPending()
{
    v.clear();
}
