#include "framework/Error.h"

#include "application/Context.h"

#include "parser/Parser.h"

#include "compiler/Compiler.h"

#include "generator/Generator.h"

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

        for(auto &n: e->entities)
        {
            Entity::print(n, std::cout);
        }

        std::cout << std::string(80, '=') << "\n";

        Compiler::compile(c, *e);

        std::cout << "strings\n";
        for(auto &s: c.strings)
        {
            std::cout << "    " << s << "\n";
        }

        std::cout << "globals\n";
        for(auto &g: c.globals)
        {
            std::cout << "    " << g.sym->name << " " << g.id << "\n";
        }

        std::cout << "functions\n";
        for(auto &f: c.functions)
        {
            std::cout << "    " << f.sym->name << " " << f.id << "\n";
        }

        Generator::generate(c, files[1]);
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
