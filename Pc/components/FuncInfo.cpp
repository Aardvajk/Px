#include "FuncInfo.h"

#include <pcx/str.h>

std::string FuncInfo::nextLabel()
{
    return pcx::str("#label", labels++);
}

std::string FuncInfo::nextLabelQuoted()
{
    return pcx::str("\"", nextLabel(), "\"");
}
