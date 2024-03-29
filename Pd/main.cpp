#include "framework/Error.h"

#include "application/Context.h"
#include "application/Process.h"

#include <pcx/format.h>
#include <pcx/datastream.h>

#include <iostream>
#include <fstream>

namespace
{

void loadFileMapIfPresent(Context &c, const std::string &path)
{
    std::ifstream is(path);
    if(is.is_open())
    {
        c.dm.read(is);
    }
}

}

int main(int argc, char *argv[])
{
    std::vector<std::string> files;
    Context c(argc, argv, files);

    try
    {
        if(files.size() < 1)
        {
            throw Error("no input specified");
        }

        auto path = files[0];

        pcx::data_ifstream is(path);
        if(!is.is_open())
        {
            throw Error("unable to open - ", path);
        }

        loadFileMapIfPresent(c, path + ".pmap");

        if(path.length() > 4 && path.substr(path.length() - 3) == ".po")
        {
            Process::unit(c, path, is, std::cout);
        }
        else if(path.length() > 4 && path.substr(path.length() - 3) == ".px")
        {
            Process::executable(c, path, is, std::cout);
        }

        std::cout << pcx::format::banner();
    }

    catch(const Error &error)
    {
        std::cerr << "pd error: " << error.what() << "\n";
        return -1;
    }
}
