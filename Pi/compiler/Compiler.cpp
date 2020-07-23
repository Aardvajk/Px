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

    auto size = e.properties["size"].to<std::size_t>();

    g->sym->properties["size"] = size;

    for(std::size_t i = 0; i < e.properties["size"].to<std::size_t>(); ++i)
    {
        g->bytes << char(0);
    }

    c.dm.begin('V', n, { });
    c.dm.setCurrentSize(size);

    c.dm("var \"", n, "\":", size);
}

template<typename T> void compilePushNumeric(Context &c, Primitive::Type type, Entity &e)
{
    auto v = e.properties["value"].to<T>();

    c.cm("-push ", Primitive::toString(type), "(", v, ");");

    c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << sizeof(T);
    c.func().bytes << OpCode::Op::CopyAI << OpCode::Reg::Sp << sizeof(T) << v;
}

void compilePush(Context &c, Entity &e)
{
    auto type = e.property("pushtype").to<std::string>();

    if(type == "numeric")
    {
        auto pt = e.property("valuetype").to<Primitive::Type>();

        switch(pt)
        {
            case Primitive::Type::Char: compilePushNumeric<char>(c, pt, e); break;
            case Primitive::Type::Int: compilePushNumeric<int>(c, pt, e); break;

            default: break;
        }
    }
    else if(type == "addrof")
    {
        auto target = e.property("target").to<std::string>();
        auto sym = c.syms.find(target);

        c.cm("-push &\"", target, "\";");

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

        c.cm("-push \"", target, "\";");

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
    auto n = e.property("amount").to<std::size_t>();

    c.cm("-pop ", n, ";");

    c.func().bytes << OpCode::Op::AddRI << OpCode::Reg::Sp << n;
}

void compileCall(Context &c, Entity &e)
{
    c.cm("-call;");

    c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Dx;
    c.func().bytes << OpCode::Op::Call << OpCode::Reg::Dx;
}

void compileLoad(Context &c, Entity &e)
{
    auto amount = e.property("amount").to<std::size_t>();

    c.cm("-load ", amount, ";");

    c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Dx;
    c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << amount;
    c.func().bytes << OpCode::Op::CopyAA << OpCode::Reg::Dx << OpCode::Reg::Sp << amount;
}

void compileStore(Context &c, Entity &e)
{
    auto amount = e.property("amount").to<std::size_t>();

    c.cm("-store ", amount, ";");

    c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Dx;
    c.func().bytes << OpCode::Op::CopyAA << OpCode::Reg::Sp << OpCode::Reg::Dx << amount;
}

void compileAllocS(Context &c, Entity &e)
{
    auto amount = e.property("amount").to<std::size_t>();

    c.cm("-allocs ", amount, ";");

    c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << amount;
}

void compileService(Context &c, Entity &e)
{
    auto code = e.property("code").to<int>();

    c.cm("-service ", code, ";");

    c.func().bytes << OpCode::Op::Svc << code;
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

        c.cm.begin('F', n, pcx::make_callback(&c, Context::currentPosition));

        c.cm("func \"", n, "\":", e.property("size").to<std::size_t>());
        c.cm("{");

        c.cm("-function prologue");

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

        c.cm("-allocate locals");
        c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << c.func().vars;

        while(index < e.entities.size())
        {
            compileInstruction(c, e.entities[index++]);
        }

        c.cm("-release locals");
        c.func().bytes << OpCode::Op::AddRI << OpCode::Reg::Sp << c.func().vars;

        c.cm("-function epilogue");
        c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Bp;
        c.func().bytes << OpCode::Op::Ret << c.func().args;

        c.cm("}");
        c.cm.setCurrentSize(c.func().bytes.position());

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
