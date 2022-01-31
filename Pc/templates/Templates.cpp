#include "Templates.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeBuilder.h"
#include "types/TypeQuery.h"

#include "decorate/Decorator.h"
#include "decorate/ArgDecorator.h"

#include "visitors/Visitor.h"
#include "visitors/NameVisitors.h"

#include <pcx/str.h>
#include <pcx/join_str.h>
#include <pcx/scoped_push.h>
#include <pcx/indexed_range.h>

#include <unordered_map>
#include <unordered_set>
#include <algorithm>

namespace
{

std::unordered_map<std::string, std::size_t> generateIndexMap(const std::vector<std::string> &names, TemplateFuncNode *node)
{
    std::unordered_map<std::string, std::size_t> map;

    for(auto a: pcx::indexed_range(node->args))
    {
        auto s = Visitor::query<NameVisitors::TypeNameExtractor, std::string>(a.value.get());

        if(std::find(names.begin(), names.end(), s) != names.end() && map.find(s) == map.end())
        {
            map.insert({ s, a.index });
        }
    }

    return map;
}

std::unordered_set<std::size_t> generateParamIndices(const std::vector<std::string> &names, TemplateFuncNode *node)
{
    std::unordered_set<std::size_t> set;

    for(auto a: pcx::indexed_range(node->args))
    {
        auto s = Visitor::query<NameVisitors::TypeNameExtractor, std::string>(a.value.get());

        if(std::find(names.begin(), names.end(), s) != names.end())
        {
            set.insert(a.index);
        }
    }

    return set;
}

void fullfillFuncReq(Context &c, const TemplateFuncReq &r)
{
    auto p = r.node->assertedProperty("sym").to<Sym*>()->property("body");
    if(!p)
    {
        throw Error(r.sym->location(), "template function not defined - ", r.sym->fullname());
    }

    auto g = pcx::scoped_push(c.templateParams, r.map);

    auto fn = new FuncNode(r.node->location());
    r.node->instances.push_back(fn);

    if(r.node->property("method").value<bool>())
    {
        fn->setProperty("method", true);
    }

    auto returnType = c.types.nullType();

    if(r.node->type)
    {
        auto copy = r.node->type->clone();
        returnType = TypeBuilder::build(c, copy.get());
    }

    auto tn = new TypeNode(r.node->location(), new IdNode(r.node->location(), { }, { }));
    fn->type = tn;

    tn->setProperty("type", returnType);

    fn->name = new IdNode(r.node->location(), { }, r.sym->name());

    NodeList args;
    for(auto a: r.node->args)
    {
        args.push_back(a->clone());
    }

    fn->args = args;
    fn->body = p.to<Node*>()->clone();

    fn->setProperty("sym", r.sym);

    Visitor::visit<Decorator>(fn, c);
}

}

Sym *Templates::generateClass(Context &c, Sym *sym, NodeList &params)
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

    if(pv.size() != params.size())
    {
        throw Error(sym->location(), "mismatched template params - ", sym->fullname());
    }

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

Sym *Templates::generateFuncReq(Context &c, Sym *sym, Type *expected, IdNode &id)
{
    auto node = sym->assertedProperty("node").to<TemplateFuncNode*>();
    auto pv = sym->property("params").to<std::vector<std::string> >();

    std::vector<Type*> types;
    for(std::size_t i = 0; i < id.params.size(); ++i)
    {
        types.push_back(TypeQuery::assert(c, id.params[i].get()));
    }

    if(types.size() < pv.size() && expected)
    {
        auto map = generateIndexMap(pv, node);

        for(std::size_t i = types.size(); i < pv.size(); ++i)
        {
            auto p = map.find(pv[i]);
            if(p != map.end())
            {
                Error::assert(p->second < expected->args.size(), id.location(), "Templates::generateFunc - p->second != expected->args.size() - ", sym->fullname());

                types.push_back(expected->args[p->second]);
            }
        }
    }

    if(pv.size() != types.size())
    {
        throw Error(id.location(), "unable to deduce template params - ", sym->fullname());
    }

    std::unordered_map<std::string, Type*> map;
    for(std::size_t i = 0; i < pv.size(); ++i)
    {
        map[pv[i]] = types[i];
    }

    auto g = pcx::scoped_push(c.templateParams, map);

    std::vector<Type*> args;

    NodeList copyArgs;
    for(auto a: node->args)
    {
        copyArgs.push_back(a->clone());
    }

    for(auto a: copyArgs)
    {
        Visitor::visit<ArgDecorator>(a.get(), c);
        args.push_back(TypeQuery::assert(c, a.get()));
    }

    auto returnType = c.types.nullType();

    if(node->type)
    {
        auto copy = node->type->clone();
        returnType = TypeBuilder::build(c, copy.get());
    }

    if(returnType->ref)
    {
        returnType = c.types.insert(returnType->addPointer());
    }

    auto t = Type::function(returnType, args);
    if(node->property("method").value<bool>())
    {
        t.method = true;
    }

    auto type = c.types.insert(t);

    auto name = pcx::str(sym->name(), "<", pcx::join_str(types, ","), ">(", pcx::join_str(args, ","), "):", returnType->description());

    if(auto s = sym->parent()->child(name))
    {
        return s;
    }

    auto s = sym->parent()->add(new Sym(Sym::Type::Func, node->location(), name));

    s->setProperty("type", type);
    s->setProperty("paramindices", generateParamIndices(pv, node));

    c.templateFuncReqs.push_back(TemplateFuncReq(node, s, map));

    return s;
}

void Templates::fullfillFuncReqs(Context &c)
{
    while(!c.templateFuncReqs.empty())
    {
        auto reqs = c.templateFuncReqs;
        c.templateFuncReqs.clear();

        for(auto &r: reqs)
        {
            if(r.used)
            {
                fullfillFuncReq(c, r);
            }
        }
    }
}
