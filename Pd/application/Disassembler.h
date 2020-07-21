#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <pcx/datastream.h>

#include <iostream>

class Context;
class ByteReader;

namespace Disassembler
{

std::string bytes(ByteReader &rm, std::size_t n);
void disassemble(Context &c, const char *data, std::size_t size, std::ostream &os);

}

#endif // DISASSEMBLER_H
