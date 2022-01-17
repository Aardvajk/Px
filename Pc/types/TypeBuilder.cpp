#include "TypeBuilder.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "visitors/NameVisitors.h"

#include "syms/SymFinder.h"

namespace
{

Type *checkTemplateParam(Context &c, NodePtr &node)
{
    if(Visitor::query<NameVisitors::IsNameSimple, bool>(node.get()))
    {
        auto s = Visitor::query<NameVisitors::TrailingId, std::string>(node.get());

        auto p = c.templateParams.back().find(s);
        if(p != c.templateParams.back().end())
        {
            return p->second;
        }
    }

    return nullptr;
}

}

TypeBuilder::TypeBuilder(Context &c) : c(c), r(nullptr)
{
}

void TypeBuilder::visit(TypeNode &node)
{
    if(!c.templateParams.empty())
    {
        r = checkTemplateParam(c, node.name);

        if(r)
        {
            if(node.ref)
            {
                auto t = *r;
                t.ref = true;

                r = c.types.insert(t);
            }
        }
    }

    if(!r)
    {
        std::vector<Sym*> sv;
        SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), node.name.get(), sv);

        if(sv.empty() || sv.front()->type() != Sym::Type::Class)
        {
            throw Error(node.location(), "type expected - ", node.description());
        }

        auto t = Type::primary(sv.front());

        t.ref = node.ref;
        t.ptr = node.ptr;

        r = c.types.insert(t);
    }
}

Type *TypeBuilder::build(Context &c, Node *node)
{
    return Visitor::query<TypeBuilder, Type*>(node, c);
}
