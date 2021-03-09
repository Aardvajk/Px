#include "framework/Error.h"

#include "application/Context.h"

#include "components/Entity.h"

#include "compiler/Parser.h"
#include "compiler/Compiler.h"

#include <iostream>
#include <vector>

void printEntities(std::ostream &os, const Entity &e, int indent);

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

        printEntities(std::cout, *n, 0);

        std::cout << "================================\n";

        Compiler::compile(c, *n);

        std::cout << "functions:\n";
        for(auto f: c.functions)
        {
            std::cout << "    " << c.strings[f.id] << "\n";
        }
    }

    catch(const Error &e)
    {
        std::cerr << "pi error";

        if(auto n = e.location())
        {
            std::cerr << " " << files[0] << " " << n.line() << "," << n.column();
        }

        std::cerr << ": " << e.what() << "\n";

        return -1;
    }
}

void printEntities(std::ostream &os, const Entity &e, int indent)
{
    os << std::string(indent * 4, ' ') << Entity::toString(e.type) << ": " << e.property("name").value<std::string>() << "\n";

    for(auto &n: e.entities)
    {
        printEntities(os, n, indent + 1);
    }
}

