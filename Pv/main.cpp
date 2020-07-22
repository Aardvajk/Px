#include "framework/Error.h"
#include "framework/ByteReader.h"
#include "framework/Console.h"

#include "application/Machine.h"

#include <pcx/args.h>

#include <vector>
#include <iostream>
#include <fstream>

namespace
{

void serviceProc(int code, Memory &mm, Registers &rg)
{
    auto sp = rg.sp();
    ByteReader rm(mm(0), sp);

    if(code == 1)
    {
        std::cout << "integer " << rm.get<int>() << "\n";
    }
    else
    {
        std::cout << "service " << code << "\n";
    }
}

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

        Machine m({ std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>() }, serviceProc);

        std::cout << banner("executing");
        m.execute();

        std::cout << banner("shutdown");
        m.shutdown();
    }

    catch(const Error &error)
    {
        std::cerr << "pv error: " << error.what() << "\n";
        return -1;
    }
}
