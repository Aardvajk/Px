#include "Context.h"

#include "framework/Error.h"

#include <fstream>

Context::Context(int argc, char *argv[], std::vector<std::string> &files) : args(argc, argv, files), scanner(Lexer::Mode::Pi)
{
}

void Context::open(const std::string &path)
{
    pcx::scoped_ptr<std::ifstream> is(new std::ifstream(path));
    if(!is->is_open())
    {
        throw Error(Location(), "unable to open - ", path);
    }

    scanner.push(new Source(sources.id(path), is.release()));
}

std::size_t Context::currentPosition() const
{
    return functions.back().bytes.position();
}
