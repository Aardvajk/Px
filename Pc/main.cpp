#include "framework/Error.h"

#include "application/Context.h"

#include "parser/Parser.h"

#include "visitors/AstPrinter.h"

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

        auto n = Parser::build(c);

        Visitor::visit<AstPrinter>(n.get(), c, std::cout);
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
