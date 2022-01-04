#include "framework/Error.h"

#include "application/Context.h"

#include "parser/Parser.h"

#include "visitors/AstPrinter.h"

#include "decorate/Decorator.h"

#include "finalise/Finaliser.h"

#include "generate/Generator.h"

#include "syms/SymPrinter.h"

#include <pcx/format.h>

#include <iostream>
#include <fstream>

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

        auto n = Parser::build(c);

        if(!c.args.contains("q"))
        {
            std::cout << pcx::format::banner("ast");
            Visitor::visit<AstPrinter>(n.get(), c, std::cout);
        }

        Visitor::visit<Decorator>(n.get(), c);

        if(!c.args.contains("q"))
        {
            std::cout << pcx::format::banner("decorated ast");
            Visitor::visit<AstPrinter>(n.get(), c, std::cout);

            std::cout << pcx::format::banner("tree");
            SymPrinter::print(c, c.tree.root(), std::cout);

            std::cout << pcx::format::banner("types");
            for(auto t: c.types)
            {
                std::cout << t.description() << "\n";
            }
        }

        Visitor::visit<Finaliser>(n.get(), c);

        if(true)
        {
            std::ofstream os(files[1]);
            if(!os.is_open())
            {
                throw Error("unable to create - ", files[1]);
            }

            Visitor::visit<Generator>(n.get(), c, os);
        }

        if(!c.args.contains("q"))
        {
            std::cout << pcx::format::banner();
        }
    }

    catch(const Error &error)
    {
        if(error.flags() & Error::Flag::Internal)
        {
            std::cerr << "internal ";
        }

        std::cerr << "pc error";
        if(auto n = error.location())
        {
            std::cerr << " " << c.sources.path(n.id()) << " " << n.line() << "," << n.column();
        }

        std::cerr << ": " << error.what() << "\n";
        return -1;
    }
}
