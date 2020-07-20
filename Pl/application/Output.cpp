#include "Output.h"

#include "framework/Error.h"

#include "application/Context.h"

#include <pcx/datastream.h>

void Output::create(Context &c, const std::string &path)
{
    pcx::data_ofstream os(path);
    if(!os.is_open())
    {
        throw Error("unable to create - ", path);
    }

    os.write(c.ds.data().data(), c.ds.position());
}
