#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

ExprGenerator::ExprGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void ExprGenerator::visit(IdNode &node)
{
    auto sym = node.assertedProperty("sym").to<Sym*>();

    if(sym->type() == Sym::Type::Func)
    {
        os << "    push &\"" << sym->funcname() << "\";\n";
        r = sizeof(std::size_t);
    }
    else
    {
        auto type = sym->assertedProperty("type").to<Type*>();

        os << "    push \"" << sym->fullname() << "\";\n";
        r = type->assertedSize(node.location());
    }
}

std::size_t ExprGenerator::generate(Context &c, std::ostream &os, Node *node)
{
    ExprGenerator g(c, os);
    node->accept(g);

    if(!g.result())
    {
        throw Error(node->location(), "expr generator failed - ", node->description());
    }

    return *g.result();
}
