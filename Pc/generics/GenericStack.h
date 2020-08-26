#ifndef GENERICSTACK_H
#define GENERICSTACK_H

#include "generics/GenericParam.h"
#include "generics/GenericRef.h"

#include <pcx/optional.h>

#include <vector>

using GenericParamList = std::vector<GenericParam>;

class Node;

class GenericStack
{
public:
    using value_type = GenericParamList;

    GenericStack();

    void push_back(const GenericParamList &g);
    void pop_back();

    pcx::optional<GenericRef> typeRef(Node *name) const;

private:
    std::vector<GenericParamList> v;
};

#endif // GENERICSTACK_H
