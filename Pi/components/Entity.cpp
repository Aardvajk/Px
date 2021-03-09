#include "Entity.h"

Entity::Entity() : type(Type::Invalid)
{
}

Entity::Entity(Type type, Location location) : type(type), location(location)
{
}

pcx::any Entity::property(const std::string &name) const
{
    auto i = properties.find(name);
    if(i != properties.end())
    {
        return i->second;
    }

    return { };
}

const char *Entity::toString(Type v)
{
    static const char *s[] =
    {
        "block",
        "func",
        "(invalid)"
    };

    return s[static_cast<int>(v)];
}
