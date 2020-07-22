#include "framework/Error.h"
#include "framework/Console.h"

#include <pcx/args.h>
#include <pcx/textfile.h>

#include <iostream>
#include <cctype>

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

                std::string exe, param;
                is >> exe >> param;

                if(std::system(pcx::str("C:/Projects/Px/Px/build-", exe, "/release/", s).c_str()))
                {
                    return -1;
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
