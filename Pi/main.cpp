#include "framework/Error.h"

#include "application/Context.h"

#include "components/Entity.h"

#include "compiler/Parser.h"
#include "compiler/Compiler.h"

#include <pcx/datastream.h>

#include <iostream>
#include <vector>

void generate(Context &c, const std::string &path);

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

        Compiler::compile(c, *n);

        generate(c, files[1]);
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

void generate(Context &c, const std::string &path)
{
    pcx::data_ofstream os(path);
    if(!os.is_open())
    {
        throw Error("unable to create - ", path);
    }

    os << std::uint32_t(0);

    os << c.strings.size();
    for(auto &s: c.strings)
    {
        os << s;
    }

    os << c.functions.size();
    for(auto &f: c.functions)
    {
        os << 'F' << std::uint32_t(0) << f.id;

        os << f.bytes.position();
        os.write(f.bytes.data().data(), f.bytes.position());
    }
}
