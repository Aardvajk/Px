#include "Parser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/DeclarationParser.h"

#include <pcx/scoped_push.h>

void Parser::construct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwNamespace:
        case Token::Type::RwFunc: DeclarationParser::build(c, block, false); break;

        default: throw Error(tok.location(), "parse error - ", tok.text());
    }
}

NodePtr Parser::build(Context &c)
{
    auto cg = pcx::scoped_push(c.containers, Sym::Type::Namespace);

    auto block = new BlockNode({ });
    NodePtr n(block);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::Eof)
    {
        construct(c, block, false);
    }

    return n;
}
