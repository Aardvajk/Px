#include "Entity.h"

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
    static const char *s[] = { "block", "global", "func", "var", "arg", "label", "instruction", "(invalid)" };
    return s[static_cast<int>(v)];
}
