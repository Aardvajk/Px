#include "framework/Error.h"

#include "application/Context.h"

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
