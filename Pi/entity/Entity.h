#ifndef ENTITY_H
#define ENTITY_H

#include <pcx/ptr_vector.h>
#include <pcx/any.h>
#include <pcx/scoped_ptr.h>

#include <string>
#include <unordered_map>
#include <iostream>

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
        Invalid
    };

    explicit Entity(Type type = Type::Invalid);

    pcx::any property(const std::string &name) const;

    static const char *toString(Type v);
    static void print(const Entity &entity, std::ostream &os);

    Type type;
    std::unordered_map<std::string, pcx::any> properties;
    pcx::ptr_vector<Entity> entities;
};

using EntityPtr = pcx::scoped_ptr<Entity>;

#endif // ENTITY_H
