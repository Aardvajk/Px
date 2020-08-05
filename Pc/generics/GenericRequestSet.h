#ifndef GENERICREQUESTSET_H
#define GENERICREQUESTSET_H

#include "generics/GenericRequest.h"

class GenericRequestSet
{
public:
    using iterator = std::vector<GenericRequest>::iterator;

    GenericRequestSet();

    void insert(const GenericRequest &request);
    void clearPending();

    std::vector<GenericRequest> current() const { return v; }

    iterator begin(){ return v.begin(); }
    iterator end(){ return v.end(); }

    bool empty() const { return v.empty(); }

private:
    std::vector<GenericRequest> v;
};

#endif // GENERICREQUESTSET_H
