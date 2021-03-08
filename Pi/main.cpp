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

        c.open(files[0]);

        auto t = c.scanner.next(true);
        while(t.type() != Token::Type::Eof)
        {
            std::cout << "[" << t.text() << "]\n";
            t = c.scanner.next(true);
        }
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
