#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"

#include "parser/Parser.h"

#include "decorator/Decorator.h"

#include "generator/Generator.h"

#include "syms/SymPrinter.h"

#include "visitors/AstPrinter.h"
#include "visitors/Finaliser.h"

#include <pcx/join_str.h>
#include <pcx/textfile.h>

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
            std::cout << banner("ast");
            Visitor::visit<AstPrinter>(n.get(), c, std::cout);
        }

        Visitor::visit<Decorator>(n.get(), c);

        if(!c.args.contains("q"))
        {
            std::cout << banner("decorated ast");
            Visitor::visit<AstPrinter>(n.get(), c, std::cout);
        }

        Visitor::visit<Finaliser>(n.get(), c);

        if(!c.args.contains("q"))
        {
            std::cout << banner("decorated ast");
            Visitor::visit<AstPrinter>(n.get(), c, std::cout);

            std::cout << banner("tree");
            SymPrinter::print(c, c.tree.root(), std::cout);
        }

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
            std::vector<std::string> t;
            pcx::textfile::read(files[1], t);

            std::cout << banner("output");
            for(auto &s: t)
            {
                std::cout << s << "\n";
            }
        }

        if(c.args.contains("execute"))
        {
            if(std::system(pcx::str("C:/Projects/Px/Px/build-Pi/release/Pi -q source.pi source.po").c_str())) return -1;
            if(std::system(pcx::str("C:/Projects/Px/Px/build-Pl/release/Pl -q -trim -entrypoint=\"main():std.null\" out.px source.po ../lib/stdlib.po ../lib/stdtest.po").c_str())) return -1;
            if(std::system(pcx::str("C:/Projects/Px/Px/build-Pv/release/Pv out.px").c_str())) return -1;
        }

        if(!c.args.contains("q"))
        {
            std::cout << banner();
        }
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
