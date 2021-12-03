#include "framework/Error.h"

#include "application/Context.h"

#include "parser/Parser.h"

#include "visitors/AstPrinter.h"

#include "syms/SymPrinter.h"

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

        c.open(files[0]);

        auto n = Parser::build(c);

        if(!c.args.contains("q"))
        {
            std::cout << pcx::format::banner("ast");
            Visitor::visit<AstPrinter>(n.get(), c, std::cout);
        }

        if(!c.args.contains("q"))
        {
            std::cout << pcx::format::banner("tree");
            SymPrinter::print(c, c.tree.root(), std::cout);
        }

        if(!c.args.contains("q"))
        {
            std::cout << pcx::format::banner();
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
