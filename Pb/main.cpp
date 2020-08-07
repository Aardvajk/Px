#include "framework/Error.h"
#include "framework/Console.h"

#include <pcx/args.h>
#include <pcx/str.h>
#include <pcx/textfile.h>

#include <iostream>
#include <cctype>

namespace
{

int test(const std::string &name)
{
    if(std::system(pcx::str("C:/Projects/Px/Px/build-Pc/release/pc -q -I+=\"../lib\" ../tests/", name, ".pc test.pi").c_str())) return -1;
    if(std::system(pcx::str("C:/Projects/Px/Px/build-Pi/release/pi test.pi test.po").c_str())) return -1;
    if(std::system(pcx::str("C:/Projects/Px/Px/build-Pl/release/pl -entrypoint=\"main():std.null\" test.px test.po ../lib/stdlib.po ../lib/stdtest.po").c_str())) return -1;
    if(std::system(pcx::str("C:/Projects/Px/Px/build-Pv/release/pv -q test.px").c_str())) return -1;

    return 0;
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
            throw Error("no source specified");
        }

        std::vector<std::string> v;
        if(!pcx::textfile::read(files[0], v))
        {
            throw Error("unable to open - ", files[0]);
        }

        std::cout << banner();

        for(auto s: v)
        {
            if(s.length() > 1 && !std::isspace(s[0]))
            {
                std::cout << "> " << s << "\n";

                std::istringstream is(s);

                std::string exe;
                is >> exe;

                if(exe == "test")
                {
                    std::string name;
                    is >> name;

                    if(test(name))
                    {
                        return -1;
                    }
                }
                else
                {
                    if(std::system(pcx::str("C:/Projects/Px/Px/build-", exe, "/release/", s).c_str()))
                    {
                        return -1;
                    }
                }
            }
        }

        std::cout << banner();
    }

    catch(const Error &e)
    {
        std::cerr << "pb error: " << e.what() << "\n";
        return -1;
    }
}
