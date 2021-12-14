#ifndef EXPRGENERATOR_H
#define EXPRGENERATOR_H

#include "visitors/Visitor.h"

#include <pcx/flags.h>
#include <pcx/optional.h>

#include <iostream>

class Context;
class Node;

class ExprGenerator : public Visitor
{
public:
    enum class Flag
    {
        Addr = 1
    };

    using Flags = pcx::flags<Flag>;

    ExprGenerator(Context &c, std::ostream &os, Flags flags = { });

    pcx::optional<std::size_t> result() const { return r; }

    virtual void visit(IdNode &node) override;

    static std::size_t generate(Context &c, std::ostream &os, Node *node, Flags flags = { });

private:
    Context &c;
    std::ostream &os;
    Flags flags;

    pcx::optional<std::size_t> r;
};

template<> struct pcx_is_flag_enum<ExprGenerator::Flag> : std::true_type { };

#endif // EXPRGENERATOR_H
