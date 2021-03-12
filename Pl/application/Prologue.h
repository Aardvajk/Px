#ifndef PROLOGUE_H
#define PROLOGUE_H

#include <pcx/datastream.h>

class Context;

namespace Prologue
{

pcx::data_ostream_patch<std::size_t> generate(Context &c);

}

#endif // PROLOGUE_H
