#include "DirectiveParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "parser/Parser.h"

#include <pcx/str.h>
#include <pcx/filesys.h>

namespace
{

std::string search(const std::vector<std::string> &search, const std::string &path)
{
    for(auto s: search)
    {
        auto p = pcx::str(s, '/', path);
        if(pcx::filesys::exists(p))
        {
            return p;
        }
    }

    return { };
}

}

void DirectiveParser::buildInclude(Context &c, BlockNode *block, bool get)
{
    c.scanner.match(Token::Type::RwInclude, get);

    auto tok = c.scanner.match(Token::Type::StringLiteral, true);

    if(c.parseInfo.containers.back() != Sym::Type::Namespace)
    {
        throw Error(tok.location(), "invalid include - ", tok.text());
    }

    auto path = tok.text();

    if(!pcx::filesys::exists(path))
    {
        path = search(c.args["I"], path);
        if(path.empty())
        {
            throw Error(tok.location(), "unable to locate - \"", tok.text(), "\"");
        }
    }

    if(!c.sources.contains(path))
    {
        c.open(path);

        c.scanner.next(true);
        while(c.scanner.token().type() != Token::Type::Eof)
        {
            Parser::construct(c, block, false);
        }

        c.scanner.pop();
    }

    c.scanner.consume(Token::Type::Semicolon, true);
}
