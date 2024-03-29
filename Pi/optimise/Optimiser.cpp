#include "Optimiser.h"

#include "application/Context.h"
#include "application/Code.h"

#include "entity/Entity.h"

namespace
{

bool hasNoEffect(Context &c, Entity &e)
{
    if(e.type == Entity::Type::Instruction)
    {
        auto code = e.property("instruction").to<Code::Type>();

        switch(code)
        {
            case Code::Type::Push: break;

            case Code::Type::Call: break;
            case Code::Type::Jump: break;

            case Code::Type::Pop:
            case Code::Type::Load:
            case Code::Type::Store:
            case Code::Type::AllocS:
            case Code::Type::IncrS: return e.property("amount").to<std::size_t>() == 0;

            case Code::Type::Service: break;

            default: break;
        }
    }

    return false;
}

bool isType(Entity &e, Code::Type code)
{
    return e.type == Entity::Type::Instruction && e.property("instruction").to<Code::Type>() == code;
}

void purgeInvalids(Context &c, Entity &e)
{
    auto i = e.entities.begin();
    while(i != e.entities.end())
    {
        if(i->type == Entity::Type::Invalid)
        {
            i = e.entities.erase(i);
        }
        else
        {
            ++i;
        }
    }
}

void collapseGroups(Context &c, Entity &e, Code::Type code)
{
    auto i = e.entities.begin();
    while(i != e.entities.end() - 1)
    {
        if(isType(*i, code))
        {
            auto n = i + 1;

            while(isType(*n, code))
            {
                auto a = i->property("amount").to<std::size_t>() + n->property("amount").to<std::size_t>();
                i->properties["amount"] = a;

                n->type = Entity::Type::Invalid;
                ++n;
            }
        }

        ++i;
    }
}

void optimiseFunc(Context &c, Entity &e)
{
    auto i = e.entities.begin();
    while(i != e.entities.end())
    {
        if(hasNoEffect(c, *i))
        {
            i = e.entities.erase(i);
        }
        else
        {
            ++i;
        }
    }

    collapseGroups(c, e, Code::Type::AllocS);
    collapseGroups(c, e, Code::Type::IncrS);

    i = e.entities.begin();
    while(i != e.entities.end() - 1)
    {
        if(isType(*i, Code::Type::Jump))
        {
            auto n = i + 1;

            if(n->type == Entity::Type::Label && n->property("name").to<std::string>() == i->property("target").to<std::string>())
            {
                i->type = Entity::Type::Invalid;
            }
        }

        ++i;
    }

    purgeInvalids(c, e);
}

}

void Optimiser::process(Context &c, Entity &root)
{
    for(auto &e: root.entities)
    {
        switch(e.type)
        {
            case Entity::Type::Func: optimiseFunc(c, e); break;

            default: break;
        }
    }
}
