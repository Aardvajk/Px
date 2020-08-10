#ifndef FUNCINFO_H
#define FUNCINFO_H

#include <string>

class FuncInfo
{
public:
    FuncInfo() : scopes(0), labels(0) { }

    std::string nextLabel();
    std::string nextLabelQuoted();

    std::size_t scopes;
    std::size_t labels;
};

#endif // FUNCINFO_H
