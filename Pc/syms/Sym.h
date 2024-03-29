#ifndef SYM_H
#define SYM_H

#include "scanner/Location.h"

#include <pcx/any.h>

#include <string>
#include <vector>
#include <unordered_map>

class Sym
{
public:
    enum class Type
    {
        Namespace,
        Func,
        TemplateFunc,
        Class,
        TemplateClass,
        Var,
        Scope,
        Invalid
    };

    Sym(Type type, Location location, std::string name);
    ~Sym();

    Sym *add(Sym *s);

    void setProperty(const std::string &name, pcx::any value);

    Sym *child(const std::string &name);
    Sym *container();

    Type type() const { return t; }
    Location location() const { return n; }

    std::vector<std::string> names() const;

    std::string name() const { return s; }
    std::string fullname() const;
    std::string funcname() const;

    Sym *parent(){ return ps; }
    const Sym *parent() const { return ps; }

    std::vector<Sym*> children(){ return cs; }
    const std::vector<Sym*> children() const { return cs; }

    pcx::any property(const std::string &name) const;
    pcx::any assertedProperty(const std::string &name) const;

    static const char *toString(Type v);

private:
    Type t;
    Location n;

    std::string s;

    Sym *ps;
    std::vector<Sym*> cs;

    std::unordered_map<std::string, pcx::any> pm;
};

#endif // SYM_H
