#include "DescVisitor.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/FuncNode.h"
#include "nodes/TypeNode.h"

DescVisitor::DescVisitor()
{
}

void DescVisitor::visit(BlockNode &node)
{
    r += "(block)";
}

void DescVisitor::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent->accept(*this);
        r += ".";
    }

    r += node.name;
}

void DescVisitor::visit(FuncNode &node)
{
    r += "func ";
    r += node.name->description();
}

void DescVisitor::visit(TypeNode &node)
{
    r += node.name->description();
}
