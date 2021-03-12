#include "Object.h"

void Object::load(pcx::data_istream &is, Entity &e)
{
    e.type = is.get<char>();
    e.flags = is.get<std::uint32_t>();
    e.id = is.get<std::size_t>();

    auto n = is.get<std::size_t>();

    e.data.resize(n);
    is.read(e.data.data(), n);
}
