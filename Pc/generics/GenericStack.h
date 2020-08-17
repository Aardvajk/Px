#ifndef GENERICSTACK_H
#define GENERICSTACK_H

#include "generics/GenericRef.h"

#include <pcx/optional.h>

#include <vector>
#include <string>

class Context;
class Node;
class Type;

class GenericParam
{
public:
    GenericParam(std::string name, Type *type) : name(std::move(name)), type(type) { }

    std::string name;
    Type *type;
};

using GenericParamList = std::vector<GenericParam>;

class GenericStack
{
public:
    using value_type = GenericParamList;

    GenericStack(){ }

    void push_back(const GenericParamList &g){ v.push_back(g); }
    void pop_back(){ v.pop_back(); }

    pcx::optional<GenericRef> typeRef(Node *name) const;

    Type *updateType(Context &c, Type *type) const;

private:
    std::vector<GenericParamList> v;
};

#endif // GENERICSTACK_H
