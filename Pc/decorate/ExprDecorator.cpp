#include "ExprDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/SymFinder.h"

ExprDecorator::ExprDecorator(Context &c, Type *expected) : c(c), expected(expected)
{
}

void ExprDecorator::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent->accept(*this);
    }

    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), &node, sv);

    if(sv.empty())
    {
        throw Error(node.location(), "symbol not found - ", node.description());
    }

    node.setProperty("sym", sv.front());
}

void ExprDecorator::visit(CallNode &node)
{
    auto type = c.types.insert(Type::function(c.types.primitiveType(Primitive::Type::Null), { }));

    Visitor::visit<ExprDecorator>(node.target.get(), c, type);
}
