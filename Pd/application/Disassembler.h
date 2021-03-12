#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <iostream>

class Context;

namespace Disassembler
{

void function(Context &c, const char *data, std::size_t size, std::ostream &os);

}

#endif // DISASSEMBLER_H
