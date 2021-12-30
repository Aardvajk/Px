#include "framework/Error.h"

#include "application/Context.h"

#include "parser/Parser.h"

#include "optimise/Optimiser.h"

#include "compiler/Compiler.h"

#include "generator/Generator.h"

#include "entity/Reconstruct.h"

#include <pcx/format.h>

#include <iostream>

int main(int argc, char *argv[])
{
    std::vector<std::string> files;
    Context c(argc, argv, files);

    try
    {
        if(files.size() < 1)
        {
            throw Error("no source specified");
        }

        if(files.size() < 2)
        {
            throw Error("no output specified");
        }

        c.open(files[0]);

        auto e = Parser::build(c);

        if(c.args.contains("o"))
        {
            Optimiser::process(c, *e);
        }

        Compiler::compile(c, *e);

        if(!c.args.contains("q"))
        {
            std::cout << "creating " << files[1] << "\n";
        }

        Generator::object(c, files[1]);

        if(c.args.contains("generate_pmap"))
        {
            Generator::map(c, files[1] + ".pmap");
        }
    }

    catch(const Error &error)
    {
        std::cerr << "pi error";
        if(auto n = error.location())
        {
            std::cerr << " " << c.sources.path(n.id()) << " " << n.line() << "," << n.column();
        }

        std::cerr << ": " << error.what() << "\n";
        return -1;
    }
}
