#include "Source.h"

Source::Source(Location::Id id, std::istream *is) : i(id), n(1), c(0), is(is), owned(true)
{
}

Source::Source(Location::Id id, std::istream &is) : i(id), n(1), c(0), is(&is), owned(false)
{
}

Source::~Source()
{
    if(owned)
    {
        delete is;
    }
}

Source::Char Source::get()
{
    auto ch = is->get();
    if(ch == '\n')
    {
        ++n;

        pc = c;
        c = 0;
    }
    else
    {
        ++c;
    }

    return ch == EOF ? Source::Char(0) : ch;
}

void Source::unget(Char ch)
{
    is->unget();
    if(ch == '\n')
    {
        --n;
        c = pc;
    }
    else
    {
        --c;
    }
}

Location Source::location() const
{
    return { i, n, c };
}
