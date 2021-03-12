#include "framework/Error.h"

#include "common/Object.h"

#include "scanner/Lexer.h"

#include "application/Context.h"
#include "application/Disassembler.h"

#include <pcx/datastream.h>
#include <pcx/format.h>
#include <pcx/indexed_range.h>

#include <iostream>

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

        pcx::data_ifstream is(files[0]);
        if(!is.is_open())
        {
            throw Error("unable to open - ", files[0]);
        }

        Object::File file(files[0]);

        std::cout << "version: " << is.get<std::uint32_t>() <<" \n";

        is >> file.strings;

        auto n = is.get<std::size_t>();
        for(std::size_t i = 0; i < n; ++i)
        {
            file.entities.push_back(new Object::Entity());
            Object::load(is, file.entities.back());
        }

        auto w = pcx::format::padw(file.strings.size());

        std::cout << "strings:\n";
        for(auto i: pcx::indexed_range(file.strings))
        {
            std::cout << pcx::format::pad(i.index, w) << ": " << Lexer::encodeString(i.value) << "\n";
        }

        std::cout << "entities:\n";
        for(auto &e: file.entities)
        {
            std::cout << e.type << " " << file.strings[e.id] << "\n";

            if(e.type == 'F')
            {
                Disassembler::function(c, e.data.data(), e.data.size(), std::cout);
            }
        }
    }

    catch(const Error &e)
    {
        std::cerr << "pd error";

        if(auto n = e.location())
        {
            std::cerr << " " << files[0] << " " << n.line() << "," << n.column();
        }

        std::cerr << ": " << e.what() << "\n";

        return -1;
    }
}
