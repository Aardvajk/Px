#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <pcx/datastream.h>

#include <iostream>

class Context;

namespace Disassembler
{

void value(Context &c, std::size_t index, const char *data, std::size_t size, std::ostream &os);
void function(Context &c, std::size_t index, const char *data, std::size_t size, std::ostream &os);

}

#endif // DISASSEMBLER_H
