#include "LocalsGenerator.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

LocalsGenerator::LocalsGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void LocalsGenerator::visit(BlockNode &node)
{
    for(auto n: node.nodes)
    {
        n->accept(*this);
    }
}

void LocalsGenerator::visit(ScopeNode &node)
{
    auto sym = node.assertedProperty("sym").to<Sym*>();

    auto g = c.tree.open(sym);
    node.body->accept(*this);
}

void LocalsGenerator::visit(VarNode &node)
{
    auto sym = node.assertedProperty("sym").to<Sym*>();
    auto type = sym->assertedProperty("type").to<Type*>();

    os << "    var \"" << sym->fullname() << "\":" << type->assertedSize(node.location()) << ";\n";
}
