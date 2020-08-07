#include "Generics.h"

#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"

#include "nodes/FuncNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "visitors/NameVisitors.h"

#include "decorator/VarDecorator.h"
#include "decorator/FuncDecorator.h"

#include "generator/Generator.h"

#include <pcx/str.h>
#include <pcx/join_str.h>
#include <pcx/indexed_range.h>
#include <pcx/scoped_push.h>

namespace
{

void fulfilRequest(Context &c, const GenericRequest &request, std::ostream &os)
{
    auto original = request.sym->assertProperty("funcnode").to<FuncNode*>();

    if(!original->body)
    {
        throw Error(original->location(), "generic function missing body - ", Generics::funcname(request.sym, request.types));
    }

    GenericParamList generics;
    for(auto n: pcx::indexed_range(original->genericTags))
    {
        generics.push_back(GenericParam(Visitor::query<NameVisitors::GenericTagName, std::string>(n.value.get()), request.types[n.index]));
    }

    auto gp = pcx::scoped_push(c.generics, generics);

    auto clone = original->clone();
    auto node = static_cast<FuncNode*>(clone.get());

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Func, node->location(), pcx::str(request.sym->fullname(), "<", pcx::join_str(request.types, ",", [](const Type *t){ return t->description(); }), ">")));

    auto type = *(request.sym->assertProperty("type").to<Type*>());

    for(auto &a: type.args)
    {
        a = c.generics.updateType(a);
    }

    type.returnType = c.generics.updateType(type.returnType);

    sym->setProperty("type", c.types.insert(type));
    sym->setProperty("funcnode", node);

    node->setProperty("sym", sym);
    node->setProperty("autogen", true);
    node->genericTags.clear();

    c.funcInfos.push_back(new FuncInfo());
    sym->setProperty("info", c.funcInfos.back_ptr());

    auto g = c.tree.open(sym);

    for(auto &a: node->args)
    {
        Visitor::visit<VarDecorator>(a.get(), c);
    }

    Visitor::visit<FuncDecorator>(node->body.get(), c);

    Visitor::visit<Generator>(node, c, os);
}

}

std::string Generics::funcname(const Sym *sym, const std::vector<Type*> &types)
{
    return pcx::str(sym->fullname(), "<", pcx::join_str(types, ",", [](const Type *t){ return t->description(); }), ">", sym->assertProperty("type").to<Type*>()->convertedDescription(types));
}

void Generics::fulfil(Context &c, std::ostream &os)
{
    while(!c.genericRequests.empty())
    {
        auto curr = c.genericRequests.current();
        c.genericRequests.clearPending();

        if(!c.args.contains("q"))
        {
            std::cout << banner("requests");
            for(auto &r: curr)
            {
                std::cout << r.sym->fullname() << "<" << pcx::join_str(r.types, ",", [](const Type *t){ return t->description(); }) << ">\n";
            }
        }

        for(auto &r: curr)
        {
            fulfilRequest(c, r, os);
        }
    }
}
