#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>

class Context;

namespace Output
{

void exe(Context &c, const std::string &path);
void map(Context &c, const std::string &path);

}

#endif // OUTPUT_H
