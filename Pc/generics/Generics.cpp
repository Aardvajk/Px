#include "Generics.h"

#include "framework/Console.h"

#include "application/Context.h"

#include "nodes/FuncNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "visitors/AstPrinter.h"

#include "decorator/FuncDecorator.h"

#include "generator/Generator.h"

#include <pcx/str.h>
#include <pcx/join_str.h>

namespace
{

Type *updateType(Type *type, const std::vector<Type*> &generics)
{
    if(type->gref)
    {
        type = generics[type->gref->index];
    }

    return type;
}

void fulfilRequest(Context &c, const GenericRequest &request, std::ostream &os)
{
    auto clone = request.sym->assertProperty("funcnode").to<FuncNode*>()->clone();
    auto node = static_cast<FuncNode*>(clone.get());

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Func, node->location(), pcx::str(request.sym->fullname(), "<", pcx::join_str(request.types, ",", [](const Type *t){ return t->description(); }), ">")));

    auto type = *(request.sym->assertProperty("type").to<Type*>());

    for(auto &a: type.args)
    {
        a = updateType(a, request.types);
    }

    type.returnType = updateType(type.returnType, request.types);

    sym->setProperty("type", c.types.insert(type));
    sym->setProperty("funcnode", node);

    node->setProperty("sym", sym);
    node->genericTags.clear();

    c.funcInfos.push_back(new FuncInfo());
    sym->setProperty("info", c.funcInfos.back_ptr());

    auto g = c.tree.open(sym);

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

        std::cout << banner("requests");
        for(auto &r: curr)
        {
            std::cout << r.sym->fullname() << "<" << pcx::join_str(r.types, ",", [](const Type *t){ return t->description(); }) << ">\n";
        }

        for(auto &r: curr)
        {
            fulfilRequest(c, r, os);
        }
    }
}
