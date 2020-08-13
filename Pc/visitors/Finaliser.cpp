#include "Finaliser.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "types/Type.h"

Finaliser::Finaliser(Context &c) : c(c)
{
}

void Finaliser::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void Finaliser::visit(NamespaceNode &node)
{
    auto g = c.tree.open(node.assertProperty("sym").to<Sym*>());
    node.body->accept(*this);
}

void Finaliser::visit(ClassNode &node)
{
    if(node.body && node.genericTags.empty())
    {
        auto sym = node.assertProperty("sym").to<Sym*>();

        if(true)
        {
            auto g = c.tree.open(sym);
            node.body->accept(*this);
        }

        std::size_t size = 0;

        for(auto s: sym->children())
        {
            if(s->type() == Sym::Type::Var)
            {
                s->setProperty("offset", size);
                size += Type::assertSize(s->location(), s->assertProperty("type").to<Type*>());
            }
        }

        sym->setProperty("size", size);
    }
}
