#ifndef LOADER_H
#define LOADER_H

#include <string>

class Context;

namespace Loader
{

void load(Context &c, const std::string &path);

}

#endif // LOADER_H
