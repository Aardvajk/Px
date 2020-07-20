#include "Loader.h"

#include "framework/Error.h"

#include "application/Context.h"

#include <pcx/datastream.h>

void Loader::load(Context &c, const std::string &path)
{
    pcx::data_ifstream is(path);
    if(!is.is_open())
    {
        throw Error("unable to open - ", path);
    }
}
