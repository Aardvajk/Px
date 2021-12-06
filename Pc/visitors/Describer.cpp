#include "Describer.h"

#include "nodes/Nodes.h"

Describer::Describer()
{
}

void Describer::visit(BlockNode &node)
{
    r << "(block)";
}

void Describer::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent->accept(*this);
        r << ".";
    }

    r << node.name;
}

void Describer::visit(NamespaceNode &node)
{
    r << "namespace " << node.name->description();
}

void Describer::visit(FuncNode &node)
{
    r << "func " << node.name->description() << "()";

    if(node.type)
    {
        r << ":" << node.type->description();
    }
}

void Describer::visit(ScopeNode &node)
{
    r << "(scope)";
}

void Describer::visit(TypeNode &node)
{
    r << node.name->description();
}
