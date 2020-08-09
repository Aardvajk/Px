#ifndef MATCH_H
#define MATCH_H

#include <cstdlib>

class Type;

class Match
{
public:
    Match();

    bool operator<(const Match &m) const { return exacts < m.exacts; }

    std::size_t total() const { return exacts + generics; }

    static Match create(const Type *expected, const Type *candidate);

    std::size_t exacts;
    std::size_t generics;
};

#endif // MATCH_H
