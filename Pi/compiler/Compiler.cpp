#include "Compiler.h"

#include "common/OpCodes.h"

#include "framework/Error.h"

#include "application/Context.h"
#include "application/Code.h"

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

    auto g = new Global(c.syms.add(new Sym(Sym::Type::Global, n)), c.strings.insert(n));
    c.globals.push_back(g);

    g->sym->properties["size"] = e.properties["size"].to<std::size_t>();

    for(std::size_t i = 0; i < e.properties["size"].to<std::size_t>(); ++i)
    {
        g->bytes << char(0);
    }
}

template<typename T> void compilePushNumeric(Context &c, Entity &e)
{
    c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << sizeof(T);
    c.func().bytes << OpCode::Op::CopyAI << OpCode::Reg::Sp << sizeof(T) << e.properties["value"].to<T>();
}

void compilePush(Context &c, Entity &e)
{
    auto type = e.property("pushtype").to<std::string>();

    if(type == "numeric")
    {
        switch(e.property("valuetype").to<Token::Type>())
        {
            case Token::Type::RwInt: compilePushNumeric<int>(c, e); break;

            default: break;
        }
    }
    else if(type == "addrof")
    {
        auto target = e.property("target").to<std::string>();
        auto sym = c.syms.find(target);

        if(!sym || sym->type == Sym::Type::Func || sym->type == Sym::Type::Global)
        {
            pcx::data_ostream_patch<std::size_t> p;

            c.func().bytes << OpCode::Op::SetRI << OpCode::Reg::Dx << p;
            c.func().bytes << OpCode::Op::PushR << OpCode::Reg::Dx;

            c.func().links.push_back(Object::Link(p.position(), c.strings.insert(target)));
        }
    }
    else if(type == "value")
    {
        auto target = e.property("target").to<std::string>();

        auto sym = c.syms.find(target);
        if(!sym || sym->type == Sym::Type::Global)
        {
            pcx::data_ostream_patch<std::size_t> p;

            auto size = sym ? sym->properties["size"].to<std::size_t>() : sizeof(std::size_t);

            c.func().bytes << OpCode::Op::SetRI << OpCode::Reg::Dx << p;
            c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << size;
            c.func().bytes << OpCode::Op::CopyAA << OpCode::Reg::Dx << OpCode::Reg::Sp << size;

            c.func().links.push_back(Object::Link(p.position(), c.strings.insert(target)));
        }
        else if(sym->type == Sym::Type::Func)
        {
            throw Error(e.location, "cannot push function by value - ", target);
        }
    }
}

void compilePop(Context &c, Entity &e)
{
    c.func().bytes << OpCode::Op::AddRI << OpCode::Reg::Sp << e.properties["amount"].to<std::size_t>();
}

void compileCall(Context &c, Entity &e)
{
    c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Dx;
    c.func().bytes << OpCode::Op::Call << OpCode::Reg::Dx;
}

void compileService(Context &c, Entity &e)
{
    c.func().bytes << OpCode::Op::Svc << e.property("code").to<int>();
}

void compileInstruction(Context &c, Entity &e)
{
    switch(e.property("instruction").to<Code::Type>())
    {
        case Code::Type::Push: compilePush(c, e); break;
        case Code::Type::Pop: compilePop(c, e); break;

        case Code::Type::Call: compileCall(c, e); break;

        case Code::Type::Service: compileService(c, e); break;

        default: break;
    }
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
                case Entity::Type::Instruction: compileInstruction(c, n); break;

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
