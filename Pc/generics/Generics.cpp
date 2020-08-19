#include "Generics.h"

#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"

#include "nodes/FuncNode.h"
#include "nodes/ClassNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "visitors/NameVisitors.h"
#include "visitors/Finaliser.h"

#include "decorator/Decorator.h"
#include "decorator/VarDecorator.h"
#include "decorator/FuncDecorator.h"

#include "generator/Generator.h"

#include <pcx/str.h>
#include <pcx/join_str.h>
#include <pcx/indexed_range.h>
#include <pcx/scoped_push.h>

#include "syms/SymPrinter.h"

namespace
{

inline std::string typeDescription(const Type *t){ return t->description(); }

std::string localClassname(const Sym *sym, const std::vector<Type*> &types)
{
    return pcx::str(sym->name(), "<", pcx::join_str(types, ",", typeDescription), ">");
}

Sym *fulfiTypeRequest(Context &c, Sym *base, const std::vector<Type*> &types)
{
    auto original = base->assertProperty("classnode").to<ClassNode*>();

    if(!original->body)
    {
        throw Error(original->location(), "generic class missing body - ", Generics::classname(base, types));
    }

    GenericParamList generics;
    for(auto n: pcx::indexed_range(original->genericTags))
    {
        generics.push_back(GenericParam(Visitor::query<NameVisitors::GenericTagName, std::string>(n.value.get()), types[n.index]));
    }

    auto gp = pcx::scoped_push(c.generics, generics);

    auto clone = original->clone();
    auto node = static_cast<ClassNode*>(clone.get());

    auto sym = base->parent()->add(new Sym(Sym::Type::Class, base->location(), localClassname(base, types)));

    node->setProperty("sym", sym);
    node->genericTags.clear();

    sym->setProperty("type", c.types.insert(Type::primary(sym)));
    sym->setProperty("classnode", node);
    sym->setProperty("defined", true);
    sym->setProperty("fulfilled", true);

    auto g = c.tree.open(sym);
    Visitor::visit<Decorator>(node->body.get(), c);
    Visitor::visit<Finaliser>(node, c);

    return sym;
}

void fulfilFuncRequest(Context &c, const GenericFuncRequest &request, std::ostream &os)
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

    auto type = *(request.sym->assertProperty("type").to<Type*>());

    for(auto &a: type.args)
    {
        a = c.generics.updateType(c, a);
    }

    type.returnType = c.generics.updateType(c, type.returnType);

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Func, node->location(), pcx::str(request.sym->fullname(), "<", pcx::join_str(request.types, ",", typeDescription), ">")));

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
    Decorator::checkFunctionReturned(c, *node);

    Visitor::visit<Generator>(node, c, os);
}

}

std::string Generics::funcname(const Sym *sym, const std::vector<Type*> &types)
{
    return pcx::str(sym->fullname(), "<", pcx::join_str(types, ",", typeDescription), ">", sym->assertProperty("type").to<Type*>()->convertedDescription(types));
}

std::string Generics::classname(const Sym *sym, const std::vector<Type*> &types)
{
    return pcx::str(sym->fullname(), "<", pcx::join_str(types, ",", typeDescription));
}

Sym *Generics::fulfilType(Context &c, Sym *sym, const std::vector<Type*> &types)
{
    auto name = localClassname(sym, types);

    if(auto s = sym->parent()->child(name))
    {
        return s;
    }

    if(!c.args.contains("q"))
    {
        std::cout << "class " << name << "\n";
    }

    return fulfiTypeRequest(c, sym, types);
}

void Generics::fulfilFuncs(Context &c, std::ostream &os)
{
    while(!c.genericFuncRequests.empty())
    {
        auto curr = c.genericFuncRequests.current();
        c.genericFuncRequests.clearPending();

        if(!c.args.contains("q"))
        {
            std::cout << banner("requests");
            for(auto &r: curr)
            {
                std::cout << "func " << r.sym->fullname() << "<" << pcx::join_str(r.types, ",", typeDescription) << ">\n";
            }
        }

        for(auto &r: curr)
        {
            fulfilFuncRequest(c, r, os);
        }
    }
}

bool Generics::anyGenerics(const std::vector<Type*> &types)
{
    for(auto t: types)
    {
        if(t->gref)
        {
            return true;
        }

        if(anyGenerics(t->generics))
        {
            return true;
        }
    }

    return false;
}

Type *Generics::updateTypeFromTypes(Context &c, Type *type, const std::vector<Type*> &types)
{
    if(type->gref && type->gref->index < types.size())
    {
        type = updateTypeFromTypes(c, types[type->gref->index], types);
    }

    if(anyGenerics(type->generics))
    {
        auto copy = *type;

        for(std::size_t i = 0; i < copy.generics.size(); ++i)
        {
            copy.generics[i] = updateTypeFromTypes(c, copy.generics[i], types);
        }

        type = c.types.insert(copy);
    }

    if(type->sym && !type->generics.empty() && !type->sym->property("size"))
    {
        type = c.types.insert(Type::primary(fulfilType(c, type->sym, type->generics)));
    }

    return type;
}

Type *Generics::updateTypeFromTarget(Context &c, Type *type, Node *target)
{
    if(auto p = target->property("generics"))
    {
        return updateTypeFromTypes(c, type, p.to<std::vector<Type*> >());
    }

    return type;
}

