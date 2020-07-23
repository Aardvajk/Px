#include "FileMap.h"

#include "scanner/Scanner.h"

#include <pcx/lexical_cast.h>

FileMap::FileMap()
{
}

void FileMap::begin(char type, const std::string &name, pcx::optional<pcx::callback<std::size_t> > callback)
{
    v.push_back({ type, name, 0, { } });
    cb = callback;
}

void FileMap::setCurrentSize(std::size_t size)
{
    v.back().size = size;
}

void FileMap::read(std::istream &is)
{
    Scanner sc(Lexer::Mode::Map);
    sc.push(new Source({ }, is));

    sc.next(true);
    while(sc.token().type() != Token::Type::Eof)
    {
        auto type = sc.match(Token::Type::Id, false);
        auto name = sc.match(Token::Type::StringLiteral, true);
        auto size = sc.match(Token::Type::IntLiteral, true);

        v.push_back({ type.text()[0], Lexer::decodeString(name.text()), pcx::lexical_cast<std::size_t>(size.text()), { } });

        sc.match(Token::Type::LeftBrace, true);

        sc.next(true);
        while(sc.token().type() != Token::Type::RightBrace)
        {
            auto addr = sc.match(Token::Type::IntLiteral, false);
            auto text = sc.match(Token::Type::StringLiteral, true);

            v.back().comments.push_back({ pcx::lexical_cast<std::size_t>(addr.text()), Lexer::decodeString(text.text()) });

            sc.next(true);
        }

        sc.next(true);
    }
}

void FileMap::write(std::ostream &os) const
{
    for(auto &e: v)
    {
        os << e.type << " \"" << Lexer::encodeString(e.name) << "\" " << e.size << "\n";
        os << "{\n";

        for(auto &c: e.comments)
        {
            os << "    " << c.address << " \"" << Lexer::encodeString(c.comment) << "\"\n";
        }

        os << "}\n";
    }
}
