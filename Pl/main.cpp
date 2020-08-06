#include "framework/Error.h"

#include "application/Context.h"
#include "application/Prologue.h"
#include "application/Loader.h"
#include "application/Linker.h"
#include "application/Generator.h"
#include "application/Output.h"

#include <iostream>
#include <vector>
#include <string>

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

        Generator::generate(c);
        Linker::link(c);

        auto ep = c.args["entrypoint"];
        if(ep.empty())
        {
            ep.push_back("main():std.null");
        }

        auto m = c.find(ep[0]);
        if(!m)
        {
            throw Error("entrypoint not found - ", ep[0]);
        }

        mp.assign(c.ds, m->offset + c.ds.position());

        Output::exe(c, files[0]);
        Output::map(c, files[0] + ".pmap");
    }

    catch(const Error &error)
    {
        std::cerr << "pl error: " << error.what() << "\n";
        return -1;
    }
}
