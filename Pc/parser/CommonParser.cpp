#include "CommonParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/Parser.h"
#include "parser/TypeParser.h"

namespace
{

void buildTemplateParam(Context &c, NodeList &container, bool get)
{
    container.push_back(TypeParser::build(c, get));

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        buildTemplateParam(c, container, true);
    }
}

NodePtr nameImp(Context &c, NodePtr parent, bool allowParams, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);

    c.scanner.next(true);

    auto id = new IdNode(tok.location(), parent, tok.text());
    NodePtr n(id);

    if(c.scanner.token().type() == Token::Type::Lt)
    {
        if(!allowParams)
        {
            throw Error(id->location(), "parameters not allowed in this context - ", id->name);
        }

        c.scanner.next(true);
        if(c.scanner.token().type() != Token::Type::Gt)
        {
            buildTemplateParam(c, id->params, false);
        }

        c.scanner.consume(Token::Type::Gt, false);
    }

    while(c.scanner.token().type() == Token::Type::Dot)
    {
        n = nameImp(c, n, allowParams, true);
    }

    return n;
}

}

NodePtr CommonParser::name(Context &c, bool get)
{
    return nameImp(c, { }, true, get);
}

NodePtr CommonParser::argName(Context &c, bool get)
{
    return nameImp(c, { }, false, get);
}

NodePtr CommonParser::blockContents(Context &c, bool get)
{
    c.scanner.match(Token::Type::LeftBrace, get);

    auto block = new BlockNode(c.scanner.token().location());
    NodePtr n(block);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::RightBrace)
    {
        Parser::construct(c, block, false);
    }

    c.scanner.next(true);

    return n;
}
