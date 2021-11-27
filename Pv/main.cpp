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

std::string charstr(char value)
{
    if(std::isprint(value))
    {
        return std::string(1, value);
    }

    return pcx::str("(", int(value), ")");
}

std::string boolstr(bool value)
{
    return value ? "true" : "false";
}

void serviceProc(int code, Memory &mm, Registers &rg)
{
    auto sp = rg.sp();
    ByteReader rm(mm(0), sp);

    if(code == 1)
    {
        std::cout << "int " << rm.get<int>() << "\n";
    }
    else if(code == 2)
    {
        std::cout << "char " << charstr(rm.get<char>()) << "\n";
    }
    else if(code == 3)
    {
        std::cout << "bool " << boolstr(rm.get<char>()) << "\n";
    }
    else if(code == 4)
    {
        std::cout << "ulong " << rm.get<std::size_t>() << "\n";
    }
    else if(code == 100)
    {
        auto a = rm.get<int>();
        auto b = rm.get<int>();

        if(a != b)
        {
            throw Error("std.test.same(std.int,std.int) failed with ", a, " and ", b);
        }
    }
    else if(code == 101)
    {
        auto a = rm.get<char>();
        auto b = rm.get<char>();

        if(a != b)
        {
            throw Error("std.test.same(std.char,std.char) failed with ", int(a), " and ", int(b));
        }
    }
    else if(code == 102)
    {
        auto a = rm.get<char>();
        auto b = rm.get<char>();

        if(a != b)
        {
            throw Error("std.test.same(std.bool,std.bool) failed with ", boolstr(a), boolstr(b));
        }
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
