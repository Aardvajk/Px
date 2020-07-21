#include "Generator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include <pcx/datastream.h>

void Generator::generate(Context &c, const std::string &path)
{
    pcx::data_ofstream os(path);
    if(!os.is_open())
    {
        throw Error("unable to create - ", path);
    }

    os << c.strings.size();
    for(auto &s: c.strings)
    {
        os << s;
    }

    os << c.globals.size() + c.functions.size();

    for(auto &g: c.globals)
    {
        os << 'V' << g.id;

        os << g.bytes.position();
        os.write(g.bytes.data().data(), g.bytes.position());
    }

    for(auto &f: c.functions)
    {
        os << 'F' << f.id;

        os << f.bytes.position();
        os.write(f.bytes.data().data(), f.bytes.position());
    }
}
