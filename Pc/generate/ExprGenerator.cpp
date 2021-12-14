#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

ExprGenerator::ExprGenerator(Context &c, std::ostream &os, Flags flags) : c(c), os(os), flags(flags)
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
    else if(sym->property("member").value<bool>())
    {
        generate(c, os, node.parent.get(), Flag::Addr);

        auto size = sym->assertedProperty("type").to<Type*>()->assertedSize(node.location());

        os << "    incrs " << sym->assertedProperty("offset").to<std::size_t>() << ";\n";

        if(flags & Flag::Addr)
        {
            r = sizeof(std::size_t);
        }
        else
        {
            os << "    load " << size << ";\n";
            r = size;
        }
    }
    else
    {
        if(flags & Flag::Addr)
        {
            os << "    push &\"" << sym->fullname() << "\";\n";
            r = sizeof(std::size_t);
        }
        else
        {
            auto type = sym->assertedProperty("type").to<Type*>();

            os << "    push \"" << sym->fullname() << "\";\n";
            r = type->assertedSize(node.location());
        }
    }
}

std::size_t ExprGenerator::generate(Context &c, std::ostream &os, Node *node, Flags flags)
{
    ExprGenerator g(c, os, flags);
    node->accept(g);

    if(!g.result())
    {
        throw Error(node->location(), "expr generator failed - ", node->description());
    }

    return *g.result();
}
