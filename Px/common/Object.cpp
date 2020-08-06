#include "Object.h"

void Object::load(pcx::data_istream &is, Entity &e)
{
    e.type = is.get<char>();
    e.flags = static_cast<Entity::Flag>(is.get<std::uint32_t>());

    e.id = is.get<std::size_t>();

    auto n = is.get<std::size_t>();

    for(std::size_t i = 0; i < n; ++i)
    {
        auto addr = is.get<std::size_t>();
        auto id = is.get<std::size_t>();

        e.links.push_back(Link(addr, id));
    }

    n = is.get<std::size_t>();

    e.data.resize(n);
    is.read(e.data.data(), n);
}
