#ifndef MATCH_H
#define MATCH_H

#include <vector>

class Context;
class Type;

class Match
{
public:
    Match();

    bool operator<(const Match &m) const { return exacts < m.exacts; }

    std::size_t total() const { return exacts + generics; }

    static Match create(Context &c, const Type *expected, const Type *candidate, const std::vector<Type*> &generics);

    std::size_t exacts;
    std::size_t generics;

    bool valid;
};

#endif // MATCH_H
