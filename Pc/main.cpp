#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"

#include "parser/Parser.h"

#include "decorator/Decorator.h"

#include "generator/Generator.h"

#include "syms/SymPrinter.h"

#include "visitors/AstPrinter.h"

#include <pcx/join_str.h>

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

        Visitor::visit<Decorator>(n.get(), c);

        std::cout << banner("ast");
        Visitor::visit<AstPrinter>(n.get(), c, std::cout);

        std::cout << banner("tree");
        SymPrinter::print(c, c.tree.root(), std::cout);

        std::cout << banner("requests");
        for(auto &g: c.genericRequests)
        {
            std::cout << g.sym->fullname() << "<" << pcx::join_str(g.types, ",", [](const Type *t){ return t->description(); }) << ">\n";
        }

        std::cout << banner("output");
        Visitor::visit<Generator>(n.get(), c, std::cout);

        if(true)
        {
            std::ofstream os(files[1]);
            if(!os.is_open())
            {
                throw Error("unable to create - ", files[1]);
            }

            Visitor::visit<Generator>(n.get(), c, os);
        }

        std::cout << banner();
    }

    catch(const Error &error)
    {
        std::cerr << "pc error";
        if(auto n = error.location())
        {
            std::cerr << " " << c.sources.path(n.id()) << " " << n.line() << "," << n.column();
        }

        std::cerr << ": " << error.what() << "\n";
        return -1;
    }
}
