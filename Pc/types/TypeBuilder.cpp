#include "TypeBuilder.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/SymFinder.h"

#include "types/Type.h"

#include "generics/Generics.h"

TypeBuilder::TypeBuilder(Context &c) : c(c), r(nullptr)
{
}

void TypeBuilder::visit(TypeNode &node)
{
    if(node.function)
    {
        auto type = c.types.nullType();
        if(node.returnType)
        {
            type = build(c, node.returnType.get());
        }

        auto t = Type::function(type);
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

        auto sym = sv.front();

        std::vector<Type*> generics;
        for(auto &n: node.generics)
        {
            generics.push_back(c.generics.updateType(c, build(c, n.get())));
        }

        if(auto p = sym->property("generics"))
        {
            if(p.to<GenericParamList>().size() != generics.size())
            {
                throw Error(node.location(), "mismatched generics - ", node.description());
            }

            if(!Generics::anyGenerics(generics))
            {
                sym = Generics::fulfilType(c, sym, generics);
                generics.clear();
            }
        }

        r = c.types.insert(Type::primary(sym, generics));
    }
}

Type *TypeBuilder::build(Context &c, Node *node)
{
    TypeBuilder v(c);
    node->accept(v);

    return v.result();
}
