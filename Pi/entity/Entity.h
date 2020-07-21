#ifndef ENTITY_H
#define ENTITY_H

#include "scanner/Location.h"

#include <pcx/ptr_vector.h>
#include <pcx/any.h>
#include <pcx/scoped_ptr.h>

#include <string>
#include <unordered_map>

class Entity
{
public:
    enum class Type
    {
        Block,
        Global,
        Func,
        Var,
        Arg,
        Label,
        Instruction,
        Invalid
    };

    Entity();
    Entity(Type type, Location location);

    pcx::any property(const std::string &name) const;

    static const char *toString(Type v);

    Type type;
    Location location;
    std::unordered_map<std::string, pcx::any> properties;
    pcx::ptr_vector<Entity> entities;
};

using EntityPtr = pcx::scoped_ptr<Entity>;

#endif // ENTITY_H
