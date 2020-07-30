#include "FuncDecorator.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

FuncDecorator::FuncDecorator(Context &c) : c(c)
{
}

void FuncDecorator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void FuncDecorator::visit(ScopeNode &node)
{
    node.body->accept(*this);
}
