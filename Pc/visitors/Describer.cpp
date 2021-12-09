#include "Describer.h"

#include "nodes/Nodes.h"

#include <pcx/join_str.h>

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
    r << node.name->description();
}

void Describer::visit(FuncNode &node)
{
    r << node.name->description() << "(" << pcx::join_str(node.args, ",") << ")";

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

void Describer::visit(VarNode &node)
{
    r << node.name->description();

    if(node.type)
    {
        r << ":" << node.type->description();
    }
}

void Describer::visit(ClassNode &node)
{
    r << node.name->description();
}
