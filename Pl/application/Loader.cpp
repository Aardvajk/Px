#include "Loader.h"

#include "framework/Error.h"

#include "common/Object.h"

#include "application/Context.h"

#include <pcx/datastream.h>

void Loader::load(Context &c, const std::string &path)
{
    pcx::data_ifstream is(path);
    if(!is.is_open())
    {
        throw Error("unable to open - ", path);
    }

    is.get<std::uint32_t>();

    auto file = new Object::File(path);
    c.files.push_back(file);

    is >> file->strings;

    auto n = is.get<std::size_t>();
    for(std::size_t i = 0; i < n; ++i)
    {
        file->entities.push_back(new Object::Entity());
        Object::load(is, file->entities.back());
    }
}
