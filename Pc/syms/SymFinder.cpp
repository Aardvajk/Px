#include "SymFinder.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"
#include "syms/SymScopeVisitor.h"

#include "templates/Templates.h"

namespace
{

void findIn(Context &c, Sym *scope, IdNode &node, std::vector<Sym*> &result)
{
    for(auto s: scope->children())
    {
        if(s->name() == node.name)
        {
            if(s->type() == Sym::Type::TemplateClass)
            {
                s = Templates::generateClass(c, s, node.params);
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
        SymScopeVisitor sv(c, curr);
        node.parent->accept(sv);

        std::vector<Sym*> sc;
        if(sv.result() == curr)
        {
            SymFinder::find(c, type, curr, node.parent.get(), sc);

            if(!sc.empty() && sc.front()->type() == Sym::Type::Var)
            {
                sc = { sc.front()->assertedProperty("type").to<::Type*>()->sym };
            }
        }
        else
        {
            sc = { sv.result() };
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

void SymFinder::visit(DerefNode &node)
{
    node.expr->accept(*this);
}

void SymFinder::find(Context &c, Type type, Sym *curr, Node *node, std::vector<Sym*> &result)
{
    SymFinder sf(c, type, curr, result);
    node->accept(sf);
}
