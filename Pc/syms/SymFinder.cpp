#include "SymFinder.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

#include "decorate/Decorator.h"

#include "types/TypeBuilder.h"
#include "types/TypeQuery.h"

#include <pcx/str.h>
#include <pcx/join_str.h>
#include <pcx/scoped_push.h>

namespace
{

Sym *generateTemplateClass(Context &c, Sym *sym, NodeList &params)
{
    for(auto p: params)
    {
        p->setProperty("type", TypeBuilder::build(c, p.get()));
    }

    auto name = pcx::str(sym->fullname(), "<", pcx::join_str(params, ",", [](const NodePtr &n){ return n->property("type").to<Type*>()->description(); }), ">");

    if(auto s = sym->parent()->child(name))
    {
        return s;
    }

    auto node = sym->property("node").to<TemplateClassNode*>();
    auto pv = sym->property("params").to<std::vector<std::string> >();

    auto s = sym->parent()->add(new Sym(Sym::Type::Class, node->location(), name));

    auto cn = new ClassNode(node->location());

    cn->name = new IdNode(node->location(), { }, name);
    cn->body = node->body->clone();

    cn->setProperty("sym", s);

    std::unordered_map<std::string, Type*> map;
    for(std::size_t i = 0; i < pv.size(); ++i)
    {
        map[pv[i]] = TypeQuery::assert(c, params[i].get());
    }

    auto g = pcx::scoped_push(c.templateParams, map);

    Visitor::visit<Decorator>(cn, c);

    node->instances.push_back(cn);

    return s;
}

void findIn(Context &c, Sym *scope, IdNode &node, std::vector<Sym*> &result)
{
    for(auto s: scope->children())
    {
        if(s->name() == node.name)
        {
            if(s->type() == Sym::Type::TemplateClass)
            {
                s = generateTemplateClass(c, s, node.params);
            }

            result.push_back(s);
        }
    }
}

void findFirst(Context &c, Sym *scope, IdNode &node, std::vector<Sym*> &result)
{
    while(scope)
    {
        findIn(c, scope, node, result);
        if(!result.empty())
        {
            return;
        }

        scope = scope->parent();
    }
}

void search(Context &c, SymFinder::Type type, Sym *scope, IdNode &node, std::vector<Sym*> &result)
{
    type == SymFinder::Type::Global ? findFirst(c, scope, node, result) : findIn(c, scope, node, result);
}

}

SymFinder::SymFinder(Context &c, Type type, Sym *curr, std::vector<Sym*> &result) : c(c), type(type), curr(curr), r(result)
{
}

void SymFinder::visit(IdNode &node)
{
    if(node.parent)
    {
        std::vector<Sym*> sc;
        SymFinder::find(c, type, curr, node.parent.get(), sc);

        if(!sc.empty() && sc.front()->type() == Sym::Type::Var)
        {
            sc = { sc.front()->assertedProperty("type").to<::Type*>()->sym };
        }

        for(auto s: sc)
        {
            search(c, Type::Local, s, node, r);
        }
    }
    else
    {
        search(c, type, curr, node, r);
    }
}

void SymFinder::find(Context &c, Type type, Sym *curr, Node *node, std::vector<Sym*> &result)
{
    SymFinder sf(c, type, curr, result);
    node->accept(sf);
}
