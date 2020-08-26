#ifndef GENERICREF_H
#define GENERICREF_H

#include <cstdlib>

class GenericRef
{
public:
    GenericRef(std::size_t index, std::size_t depth) : index(index), depth(depth) { }

    bool operator==(const GenericRef &g) const { return index == g.index && depth == g.depth; }
    bool operator!=(const GenericRef &g) const { return index != g.index || depth != g.depth; }

    std::size_t index;
    std::size_t depth;
};

#endif // GENERICREF_H
