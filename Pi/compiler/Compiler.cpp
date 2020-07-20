#include "Compiler.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "entity/Entity.h"

#include <pcx/scoped_push.h>

namespace
{

void compileGlobal(Context &c, Entity &e)
{
    auto n = e.property("name").to<std::string>();

    if(c.syms.findLocal(n))
    {
        throw Error(e.location, "symbol already defined - ", n);
    }

    auto g = new Global(c.syms.add(new Sym(Sym::Type::Func, n)), c.strings.insert(n));
    c.globals.push_back(g);
}

void compileArg(Context &c, Function *func, Entity &e)
{
}

void compileFunction(Context &c, Entity &e)
{
    if(e.property("defined"))
    {
        auto n = e.property("name").to<std::string>();

        if(c.syms.findLocal(n))
        {
            throw Error(e.location, "symbol already defined - ", n);
        }

        auto f = new Function(c.syms.add(new Sym(Sym::Type::Func, n)), c.strings.insert(n));
        c.functions.push_back(f);

        c.syms.push();

        for(auto &n: e.entities)
        {
            switch(n.type)
            {
                case Entity::Type::Arg: compileArg(c, f, n); break;

                default: break;
            }
        }

        c.syms.pop();
    }
}

}

void Compiler::compile(Context &c, Entity &root)
{
    for(auto &e: root.entities)
    {
        switch(e.type)
        {
            case Entity::Type::Global: compileGlobal(c, e); break;
            case Entity::Type::Func: compileFunction(c, e); break;

            default: break;
        }
    }
}
