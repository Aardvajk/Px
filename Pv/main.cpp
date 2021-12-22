#include "framework/Error.h"
#include "framework/ByteReader.h"

#include "application/Machine.h"

#include <pcx/format.h>
#include <pcx/args.h>
#include <pcx/str.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>

namespace
{

void serviceProc(int code, Memory &mm, Registers &rg)
{
    auto sp = rg.sp();
    ByteReader rm(mm(0), sp);

    if(code == 1)
    {
        std::cout << "char " << rm.get<char>() << "\n";
    }
    else if(code == 2)
    {
        std::cout << "int " << rm.get<int>() << "\n";
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

        Machine m(args, { std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>() }, serviceProc);

        if(!args.contains("q"))
        {
            std::cout << pcx::format::banner("executing");
        }

        m.execute();

        if(!args.contains("q"))
        {
            std::cout << pcx::format::banner("shutdown");
        }

        m.shutdown();
    }

    catch(const Error &error)
    {
        std::cerr << "pv error: " << error.what() << "\n";
        return -1;
    }
}
