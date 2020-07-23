#ifndef GENERATOR_H
#define GENERATOR_H

#include <string>

class Context;

namespace Generator
{

void object(Context &c, const std::string &path);
void map(Context &c, const std::string &path);

}

#endif // GENERATOR_H
