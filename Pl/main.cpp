#include "framework/Error.h"

#include "application/Context.h"
#include "application/Prologue.h"
#include "application/Loader.h"

#include <pcx/datastream.h>

#include <iostream>

int main(int argc, char *argv[])
{
    std::vector<std::string> files;
    Context c(argc, argv, files);

    try
    {
        if(files.size() < 1)
        {
            throw Error("no output specified");
        }

        if(files.size() < 2)
        {
            throw Error("no inputs specified");
        }

        auto mp = Prologue::generate(c);

        for(std::size_t i = 1; i < files.size(); ++i)
        {
            Loader::load(c, files[i]);
        }

        auto ep = c.args["entrypoint"];
        if(ep.empty())
        {
            throw Error("no entrypoint specified");
        }

        auto m = c.find(ep[0]);
        if(!m)
        {
            throw Error("entrypoint not found - ", ep[0]);
        }

        for(auto &f: c.files)
        {
            for(auto &e: f.entities)
            {
                auto &s = (e.type == 'V' ? c.ds : c.cs);

                e.offset = s.position();
                s.write(e.data.data(), e.data.size());
            }
        }

        mp.assign(c.ds, m->offset + c.ds.position());

        pcx::data_ofstream os(files[0]);
        if(!os.is_open())
        {
            throw Error("unable to create - ", files[0]);
        }

        os.write(c.ds.data().data(), c.ds.position());
        os.write(c.cs.data().data(), c.cs.position());
    }

    catch(const Error &e)
    {
        std::cerr << "pl error";

        if(auto n = e.location())
        {
            std::cerr << " " << files[0] << " " << n.line() << "," << n.column();
        }

        std::cerr << ": " << e.what() << "\n";

        return -1;
    }
}
