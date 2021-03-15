#include "framework/Error.h"

#include "application/Machine.h"

#include <pcx/args.h>

#include <iostream>
#include <fstream>

void serviceProc(int code, Memory &mm, Registers &rg)
{
    std::cout << "service " << code << "\n";
}

int main(int argc, char *argv[])
{
    try
    {
        std::vector<std::string> files;
        pcx::args args(argc, argv, files);

        if(files.size() < 1)
        {
            throw Error("no program specified");
        }

        std::ifstream is(files[0], std::ios::binary);
        if(!is.is_open())
        {
            throw Error("unable to open - ", files[0]);
        }

        Machine m(args, { std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>() }, serviceProc);

        m.execute();
        m.shutdown();
    }

    catch(const Error &e)
    {
        std::cerr << "pv error: " << e.what() << "\n";
        return -1;
    }
}
