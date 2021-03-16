#include "Compiler.h"

#include "framework/Error.h"

#include "common/Vm.h"

#include "application/Context.h"

#include "components/Entity.h"

#include <pcx/range_reverse.h>

namespace
{

void compileArg(Context &c, Entity &e)
{
    auto n = e.property("name").to<std::string>();
    if(c.syms.findLocal(n))
    {
        throw Error(e.location, "symbol already defined - ", n);
    }

    auto s = c.syms.add(new Sym(Sym::Type::Arg, n));

    s->properties["size"] = e.property("size").to<std::size_t>();
    s->properties["offset"] = c.func().args + (sizeof(std::size_t) * 2);

    c.func().args += s->property("size").to<std::size_t>();
}

void compileVar(Context &c, Entity &e)
{
    auto n = e.property("name").to<std::string>();
    if(c.syms.findLocal(n))
    {
        throw Error(e.location, "symbol already defined - ", n);
    }

    auto s = c.syms.add(new Sym(Sym::Type::Var, n));

    auto size = e.property("size").to<std::size_t>();

    s->properties["size"] = size;
    s->properties["offset"] = c.func().vars + size;

    c.func().vars += size;
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

        c.func().bytes << Vm::Op::PushR << Vm::Reg::Bp;
        c.func().bytes << Vm::Op::CopyRR << Vm::Reg::Sp << Vm::Reg::Bp;

        std::size_t index = 0;

        std::vector<Entity*> args;
        while(index < e.entities.size() && e.entities[index].type == Entity::Type::Arg)
        {
            args.push_back(e.entities.ptr(index++));
        }

        for(auto &e: pcx::range_reverse(args))
        {
            compileArg(c, *e);
        }

        while(index < e.entities.size() && e.entities[index].type == Entity::Type::Var)
        {
            compileVar(c, e.entities[index++]);
        }

        c.func().bytes << Vm::Op::SubRI << Vm::Reg::Sp << c.func().vars;

        while(index < e.entities.size())
        {
        }

        c.func().bytes << Vm::Op::AddRI << Vm::Reg::Sp << c.func().vars;

        c.func().bytes << Vm::Op::PopR << Vm::Reg::Bp;
        c.func().bytes << Vm::Op::Ret << c.func().args;

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
            case Entity::Type::Func: compileFunction(c, e); break;

            default: break;
        }
    }
}
