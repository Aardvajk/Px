#include "Generator.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

Generator::Generator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void Generator::visit(BlockNode &node)
{
    for(auto n: node.nodes)
    {
        n->accept(*this);
    }
}

void Generator::visit(NamespaceNode &node)
{
    node.body->accept(*this);
}

void Generator::visit(FuncNode &node)
{
    if(node.body)
    {
        auto sym = node.assertedProperty("sym").to<Sym*>();
        auto type = sym->assertedProperty("type").to<Type*>();

        os << "func \"" << sym->funcname() << "\":" << type->returnType->assertedSize(node.location()) << "\n";
        os << "{\n";
        os << "}\n";
    }
}
