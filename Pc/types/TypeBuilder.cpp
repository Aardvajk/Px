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
    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), node.name.get(), sv);

    if(sv.empty() || sv.front()->type() != Sym::Type::Class)
    {
        throw Error(node.location(), "type expected - ", node.description());
    }

    r = c.types.insert(Type::primary(sv.front()));
}

Type *TypeBuilder::build(Context &c, Node *node)
{
    TypeBuilder v(c);
    node->accept(v);

    return v.result();
}
