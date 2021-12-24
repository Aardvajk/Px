#include "Describer.h"

#include "nodes/Nodes.h"

#include <pcx/str.h>
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

    if(!node.params.empty())
    {
        r << "<" << pcx::join_str(node.params, ",") << ">";
    }
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

void Describer::visit(TemplateFuncNode &node)
{
    r << node.name->description() << "<" << pcx::join_str(node.params, ",") << ">(" << pcx::join_str(node.args, ",") << ")";

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

void Describer::visit(TemplateClassNode &node)
{
    r << node.name->description() << "<" << pcx::join_str(node.params, ",") << ">";
}

void Describer::visit(ExprNode &node)
{
    r << node.expr->description();
}

void Describer::visit(CallNode &node)
{
    r << node.target->description() << "(" << pcx::join_str(node.args, ",") << ")";
}

void Describer::visit(CharLiteralNode &node)
{
    r << '\'' << node.value << '\'';
}

void Describer::visit(IntLiteralNode &node)
{
    r << pcx::str(node.value);
}
