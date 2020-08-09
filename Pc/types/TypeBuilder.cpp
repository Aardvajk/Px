#include "TypeBuilder.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/SymFinder.h"

#include "types/Type.h"

TypeBuilder::TypeBuilder(Context &c) : c(c), r(nullptr)
{
}

void TypeBuilder::visit(TypeNode &node)
{
    if(node.returnType)
    {
        auto t = Type::function(build(c, node.returnType.get()));
        for(auto &a: node.args)
        {
            t.args.push_back(build(c, a.get()));
        }

        r = c.types.insert(t);
    }
    else if(auto g = c.generics.typeRef(node.name.get()))
    {
        r = c.types.insert(Type::generic(*g));
    }
    else
    {
        std::vector<Sym*> sv;
        SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), node.name.get(), sv);

        if(sv.empty() || sv.front()->type() != Sym::Type::Class)
        {
            throw Error(node.location(), "type expected - ", node.description());
        }

        r = c.types.insert(Type::primary(sv.front()));
    }
}

Type *TypeBuilder::build(Context &c, Node *node)
{
    TypeBuilder v(c);
    node->accept(v);

    return v.result();
}
