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

        for(std::size_t i = 1; i < files.size(); ++i)
        {
            Loader::load(c, files[i]);
        }

        auto mp = Prologue::generate(c);
        Generator::generate(c);
        Linker::link(c);

        auto m = c.find("main");
        if(!m)
        {
            throw Error("no main function found");
        }

        mp.assign(c.ds, m->offset + c.ds.position());

        Output::create(c, files[0]);
    }

    catch(const Error &error)
    {
        std::cerr << "pl error: " << error.what() << "\n";
        return -1;
    }
}
