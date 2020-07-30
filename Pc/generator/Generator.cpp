#include "Generator.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

#include "types/Type.h"

Generator::Generator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void Generator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void Generator::visit(NamespaceNode &node)
{
    auto g = c.tree.open(node.property("sym").to<Sym*>());
    node.body->accept(*this);
}

void Generator::visit(FuncNode &node)
{
    if(node.body)
    {
        auto sym = node.property("sym").to<Sym*>();
        auto type = sym->property("type").to<Type*>();

        os << "func \"" << sym->fullname() << type->description() << "\":" << Type::assertSize(node.location(), type->returnType) << "\n";
        os << "{\n";

        os << "}\n";
    }
}
