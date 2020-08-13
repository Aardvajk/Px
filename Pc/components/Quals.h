#ifndef QUALS_H
#define QUALS_H

#include <pcx/flags.h>

enum class Qual
{
    External = 1
};

using Quals = pcx::flags<Qual>;

template<> struct pcx_is_flag_enum<Qual> : std::true_type { };

#endif // QUALS_H
