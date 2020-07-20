#include "Entity.h"

namespace
{

void printEntity(const Entity &e, std::ostream &os, unsigned tab)
{
    os << std::string(tab * 4, ' ') << Entity::toString(e.type);

    if(auto p = e.property("name"))
    {
        os << " " << p.to<std::string>();
    }

    if(auto p = e.property("size"))
    {
        os << ":" << p.to<std::size_t>();
    }

    os << "\n";

    for(auto &n: e.entities)
    {
        printEntity(n, os, tab + 1);
    }
}

}

Entity::Entity() : type(Type::Invalid)
{
}

Entity::Entity(Type type, Location location) : type(type), location(location)
{
}

pcx::any Entity::property(const std::string &name) const
{
    auto p = properties.find(name);
    if(p != properties.end())
    {
        return p->second;
    }

    return { };
}

const char *Entity::toString(Type v)
{
    static const char *s[] = { "block", "global", "func", "var", "arg", "label", "(invalid)" };
    return s[static_cast<int>(v)];
}

void Entity::print(const Entity &entity, std::ostream &os)
{
    printEntity(entity, os, 0);
}
