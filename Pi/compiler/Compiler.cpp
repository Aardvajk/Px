#include "Compiler.h"

#include "framework/Error.h"

#include "common/Vm.h"

#include "application/Context.h"

#include "components/Entity.h"

namespace
{

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
