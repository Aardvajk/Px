#ifndef GENERICREF_H
#define GENERICREF_H

#include <cstdlib>

class GenericRef
{
public:
    explicit GenericRef(std::size_t index) : index(index) { }

    bool operator==(const GenericRef &g) const { return index == g.index; }
    bool operator!=(const GenericRef &g) const { return index != g.index; }

    std::size_t index;
};

#endif // GENERICREF_H
