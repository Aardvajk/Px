#include "DescVisitor.h"

#include "nodes/Nodes.h"

#include <pcx/join_str.h>
#include <pcx/lexical_cast.h>

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

    if(!node.generics.empty())
    {
        r += "<" + pcx::join_str(node.generics, ", ") + ">";
    }
}

void DescVisitor::visit(GenericTagNode &node)
{
    r += node.name;
}

void DescVisitor::visit(NamespaceNode &node)
{
    r += "namespace ";
    r += node.name->description();
}

void DescVisitor::visit(FuncNode &node)
{
    r += "func";

    if(!node.genericTags.empty())
    {
        r += "<" + pcx::join_str(node.genericTags, ", ") + ">";
    }

    r += " " + node.name->description();
    r += "(" + pcx::join_str(node.args, ", ") + ")";

    if(node.type)
    {
        r += ":" + node.type->description();
    }
}

void DescVisitor::visit(TypeNode &node)
{
    r += node.name->description();
}

void DescVisitor::visit(VarNode &node)
{
    r += node.name->description();

    if(node.type)
    {
        r += ":" + node.type->description();
    }
}

void DescVisitor::visit(ScopeNode &node)
{
    r += "(scope)";
}

void DescVisitor::visit(ExprNode &node)
{
    node.expr->accept(*this);
}

void DescVisitor::visit(CallNode &node)
{
    node.target->accept(*this);
    r += "(" + pcx::join_str(node.args, ", ") + ")";
}

void DescVisitor::visit(CharLiteralNode &node)
{
    r += node.value;
}

void DescVisitor::visit(IntLiteralNode &node)
{
    r += pcx::lexical_cast<std::string>(node.value);
}
