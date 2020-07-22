#include "Compiler.h"

#include "common/OpCodes.h"
#include "common/Primitives.h"

#include "framework/Error.h"

#include "application/Context.h"
#include "application/Code.h"

#include "entity/Entity.h"

#include <pcx/scoped_push.h>
#include <pcx/range_reverse.h>

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
        switch(e.property("valuetype").to<Primitive::Type>())
        {
            case Primitive::Type::Int: compilePushNumeric<int>(c, e); break;

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
        else if(sym->type == Sym::Type::Arg || sym->type == Sym::Type::Var)
        {
            c.func().bytes << OpCode::Op::CopyRR << OpCode::Reg::Bp << OpCode::Reg::Dx;
            c.func().bytes << (sym->type == Sym::Type::Arg ? OpCode::Op::AddRI : OpCode::Op::SubRI) << OpCode::Reg::Dx << sym->property("offset").to<std::size_t>();
            c.func().bytes << OpCode::Op::PushR << OpCode::Reg::Dx;
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
        else if(sym->type == Sym::Type::Arg || sym->type == Sym::Type::Var)
        {
            auto size = sym->property("size").to<std::size_t>();

            c.func().bytes << OpCode::Op::CopyRR << OpCode::Reg::Bp << OpCode::Reg::Dx;
            c.func().bytes << (sym->type == Sym::Type::Arg ? OpCode::Op::AddRI : OpCode::Op::SubRI) << OpCode::Reg::Dx << sym->property("offset").to<std::size_t>();
            c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << size;
            c.func().bytes << OpCode::Op::CopyAA << OpCode::Reg::Dx << OpCode::Reg::Sp << size;
        }
        else if(sym->type == Sym::Type::Func)
        {
            throw Error(e.location, "cannot push function by value - ", target);
        }
    }
}

void compilePop(Context &c, Entity &e)
{
    c.func().bytes << OpCode::Op::AddRI << OpCode::Reg::Sp << e.property("amount").to<std::size_t>();
}

void compileCall(Context &c, Entity &e)
{
    c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Dx;
    c.func().bytes << OpCode::Op::Call << OpCode::Reg::Dx;
}

void compileLoad(Context &c, Entity &e)
{
    auto amount = e.property("amount").to<std::size_t>();

    c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Dx;
    c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << amount;
    c.func().bytes << OpCode::Op::CopyAA << OpCode::Reg::Dx << OpCode::Reg::Sp << amount;
}

void compileStore(Context &c, Entity &e)
{
    auto amount = e.property("amount").to<std::size_t>();

    c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Dx;
    c.func().bytes << OpCode::Op::CopyAA << OpCode::Reg::Sp << OpCode::Reg::Dx << amount;
}

void compileAllocS(Context &c, Entity &e)
{
    c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << e.property("amount").to<std::size_t>();
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

        case Code::Type::Load: compileLoad(c, e); break;
        case Code::Type::Store: compileStore(c, e); break;

        case Code::Type::AllocS: compileAllocS(c, e); break;

        case Code::Type::Service: compileService(c, e); break;

        default: break;
    }
}

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

    s->properties["size"] = e.property("size").to<std::size_t>();
    s->properties["offset"] = c.func().vars;

    c.func().vars += s->property("size").to<std::size_t>();
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

        auto r = c.syms.add(new Sym(Sym::Type::Arg, "@ret"));

        r->properties["size"] = e.property("size").to<std::size_t>();
        r->properties["offset"] = c.func().args + (sizeof(std::size_t) * 2);

        c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << c.func().vars;

        while(index < e.entities.size())
        {
            compileInstruction(c, e.entities[index++]);
        }

        c.func().bytes << OpCode::Op::AddRI << OpCode::Reg::Sp << c.func().vars;

        c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Bp;
        c.func().bytes << OpCode::Op::Ret << c.func().args;

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
