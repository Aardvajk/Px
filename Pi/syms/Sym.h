#ifndef SYM_H
#define SYM_H

#include <pcx/any.h>
#include <pcx/scoped_ptr.h>

#include <string>
#include <unordered_map>

class Sym
{
public:
    enum class Type
    {
        Func,
        Invalid
    };

    Sym(Type type, std::string name);

    pcx::any property(const std::string &name) const;

    static const char *toString(Type v);

    Type type;
    std::string name;
    std::unordered_map<std::string, pcx::any> properties;
};

#endif // SYM_H
