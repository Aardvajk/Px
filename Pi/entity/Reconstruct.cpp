#include "Reconstruct.h"

#include "application/Context.h"
#include "application/Code.h"

#include "entity/Entity.h"

namespace
{

void outputPush(Context &c, Entity &e, std::ostream &os)
{
    os << " ";

    auto type = e.property("pushtype").to<std::string>();

    if(type == "addrof" || type == "value")
    {
        if(type == "addrof")
        {
            os << "&";
        }

        os << "\"" << e.property("target").to<std::string>() << "\"";
    }
}

void outputVar(Context &c, Entity &e, std::ostream &os)
{
    os << "    " << Entity::toString(e.type) << " \"" << e.property("name").to<std::string>() << "\":" << e.property("size").to<std::size_t>() << ";\n";
}

void outputCode(Context &c, Entity &e, std::ostream &os)
{
    auto code = e.property("instruction").to<Code::Type>();

    os << "    " << Code::toString(code);

    switch(code)
    {
        case Code::Type::Push: outputPush(c, e, os); break;

        case Code::Type::Call: break;
        case Code::Type::Jump: break;

        case Code::Type::Pop:
        case Code::Type::Load:
        case Code::Type::Store:
        case Code::Type::AllocS:
        case Code::Type::IncrS: os << " " << e.property("amount").to<std::size_t>(); break;

        case Code::Type::Service: os << " " << e.property("code").to<int>(); break;

        default: break;
    }

    os << ";\n";
}

void outputFunc(Context &c, Entity &e, std::ostream &os)
{
    os << "func \"" << e.property("name").to<std::string>() << "\":" << e.property("size").to<std::size_t>() << "\n";
    os << "{\n";

    for(auto &i: e.entities)
    {
        switch(i.type)
        {
            case Entity::Type::Arg:
            case Entity::Type::Var: outputVar(c, i, os); break;

            case Entity::Type::Instruction: outputCode(c, i, os); break;

            default: break;
        }
    }

    os << "}\n";
}

void output(Context &c, Entity &e, std::ostream &os)
{
    switch(e.type)
    {
        case Entity::Type::Func: outputFunc(c, e, os);

        default: break;
    }
}

}

void Reconstruct::toStream(Context &c, Entity &root, std::ostream &os)
{
    for(auto &e: root.entities)
    {
        output(c, e, os);
    }
}
