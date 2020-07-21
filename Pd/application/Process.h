#ifndef PROCESS_H
#define PROCESS_H

#include <pcx/datastream.h>

class Context;

namespace Process
{

void unit(Context &c, const std::string &path, pcx::data_istream &is, std::ostream &os);
void executable(Context &c, const std::string &path, pcx::data_istream &is, std::ostream &os);

}

#endif // PROCESS_H
