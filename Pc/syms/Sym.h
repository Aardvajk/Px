#ifndef SYM_H
#define SYM_H

#include "scanner/Location.h"

#include <string>
#include <vector>

class Sym
{
public:
    enum class Type
    {
        Namespace,
        Invalid
    };

    Sym(Type type, Location location, std::string name);
    ~Sym();

    Sym *add(Sym *sym);

    Type type() const { return t; }
    Location location() const { return n; }

    std::string name() const { return s; }

    Sym *parent(){ return ps; }
    const Sym *parent() const { return ps; }

    std::vector<Sym*> children(){ return cs; }
    const std::vector<Sym*> children() const { return cs; }

    static const char *toString(Type v);

private:
    Type t;
    Location n;
    std::string s;

    Sym *ps;
    std::vector<Sym*> cs;
};

#endif // SYM_H
