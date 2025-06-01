#ifndef ERROR_H
#define ERROR_H

#include "scanner/Location.h"

#include <pcx/str.h>
#include <pcx/flags.h>

#include <stdexcept>

class Error : public std::runtime_error
{
public:
    enum class Flag
    {
        Internal = 1
    };

    using Flags = pcx::flags<Flag>;

    template<typename... Args> Error(Args&&... args) : std::runtime_error(pcx::str(std::forward<Args>(args)...)), n({ }) { }
    template<typename... Args> Error(Location location, Args&&... args) : std::runtime_error(pcx::str(std::forward<Args>(args)...)), n(location) { }
    template<typename... Args> Error(Flag flag, Location location, Args&&... args) : std::runtime_error(pcx::str(std::forward<Args>(args)...)), n(location), f(flag) { }
    template<typename... Args> Error(Flags flags, Location location, Args&&... args) : std::runtime_error(pcx::str(std::forward<Args>(args)...)), n(location), f(flags) { }

    Location location() const { return n; }
    Flags flags() const { return f; }

    template<typename... Args> static void assert(bool condition, Location location, Args&&... args){ if(!condition) throw Error(Error::Flag::Internal, location, args...); }

private:
    Location n;
    Flags f;
};

template<> struct pcx_is_flag_enum<Error::Flag> : std::true_type { };

#endif // ERROR_H
