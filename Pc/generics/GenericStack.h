#ifndef GENERICSTACK_H
#define GENERICSTACK_H

#include "generics/GenericRef.h"

#include <pcx/optional.h>

#include <vector>
#include <string>

class Node;

using GenericParamList = std::vector<std::string>;

class GenericStack
{
public:
    using value_type = GenericParamList;

    GenericStack(){ }

    void push_back(const GenericParamList &g){ v.push_back(g); }
    void pop_back(){ v.pop_back(); }

    pcx::optional<GenericRef> typeRef(Node *name) const;

private:
    std::vector<GenericParamList> v;
};

#endif // GENERICSTACK_H
