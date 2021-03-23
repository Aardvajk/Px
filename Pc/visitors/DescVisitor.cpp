#include "DescVisitor.h"

#include "nodes/Nodes.h"

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
    r += node.name->description() + "()";

    if(node.type)
    {
        r += ":" + node.type->description();
    }
}

void DescVisitor::visit(TypeNode &node)
{
    r += node.name->description();
}
