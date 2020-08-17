#ifndef GENERICFUNCREQUESTSET_H
#define GENERICFUNCREQUESTSET_H

#include "generics/GenericFuncRequest.h"

#include <vector>
#include <string>
#include <unordered_set>

class GenericFuncRequestSet
{
public:
    using iterator = std::vector<GenericFuncRequest>::iterator;

    GenericFuncRequestSet();

    void insert(const GenericFuncRequest &request);
    void clearPending();

    std::vector<GenericFuncRequest> current() const { return v; }

    iterator begin(){ return v.begin(); }
    iterator end(){ return v.end(); }

    bool empty() const { return v.empty(); }

private:
    std::vector<GenericFuncRequest> v;
    std::unordered_set<std::string> sc;
};

#endif // GENERICFUNCREQUESTSET_H
