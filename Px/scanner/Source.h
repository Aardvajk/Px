#ifndef SOURCE_H
#define SOURCE_H

#include "scanner/Location.h"

#include <iostream>

class Source
{
public:
    Source(Location::Id id, std::istream *is);
    Source(Location::Id id, std::istream &is);
    ~Source();

    using Char = std::istream::int_type;

    Char get();
    void unget(Char ch);

    Location location() const;

private:
    Location::Id i;
    Location::Line n;
    Location::Column c, pc;

    std::istream *is;
    bool owned;
};

#endif // SOURCE_H
