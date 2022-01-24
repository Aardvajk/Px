#ifndef QUERYVISITORS_H
#define QUERYVISITORS_H

#include "visitors/Visitor.h"

#include "nodes/Node.h"

namespace QueryVisitors
{

class GetParent : public Visitor
{
public:
    GetParent();

    NodePtr result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    NodePtr r;
};

}

#endif // QUERYVISITORS_H
