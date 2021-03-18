#include "framework/Error.h"

#include "application/Context.h"

#include "parser/Parser.h"

#include "visitors/AstPrinter.h"

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

        std::cout << pcx::format::banner("ast");
        Visitor::visit<AstPrinter>(n.get(), c, std::cout);

        std::cout << pcx::format::banner();
    }

    catch(const Error &e)
    {
        std::cerr << "pc error";

        if(auto n = e.location())
        {
            std::cerr << " " << c.sources.path(n.id()) << " " << n.line() << "," << n.column();
        }

        std::cerr << ": " << e.what() << "\n";

        return -1;
    }
}
