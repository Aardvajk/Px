#include "Output.h"

#include "framework/Error.h"

#include "application/Context.h"

#include <pcx/datastream.h>

void Output::exe(Context &c, const std::string &path)
{
    pcx::data_ofstream os(path);
    if(!os.is_open())
    {
        throw Error("unable to create - ", path);
    }

    os.write(c.ds.data().data(), c.ds.position());
    os.write(c.cs.data().data(), c.cs.position());
}

void Output::map(Context &c, const std::string &path)
{
    std::ofstream os(path);
    if(!os.is_open())
    {
        throw Error("unable to open - ", path);
    }

    c.dm.write(os);
    c.cm.write(os);
}
