#include "Compiler.h"

#include "common/OpCodes.h"

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

    for(std::size_t i = 0; i < e.properties["size"].to<std::size_t>(); ++i)
    {
        g->bytes << char(0);
    }
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

        c.func().bytes << OpCode::Op::PushR << OpCode::Reg::Bp;
        c.func().bytes << OpCode::Op::CopyRR << OpCode::Reg::Sp << OpCode::Reg::Bp;

        for(auto &n: e.entities)
        {
            switch(n.type)
            {
                case Entity::Type::Arg: compileArg(c, f, n); break;

                default: break;
            }
        }

        c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Bp;
        c.func().bytes << OpCode::Op::Ret << std::size_t(0);

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
