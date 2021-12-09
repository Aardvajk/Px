#ifndef FUNCINFO_H
#define FUNCINFO_H

#include <cstddef>

class FuncInfo
{
public:
    FuncInfo() : scopes(0) { }

    std::size_t scopes;
};

#endif // FUNCINFO_H
