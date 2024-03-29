#ifndef GENERATOR_H
#define GENERATOR_H

#include "visitors/Visitor.h"

#include <iostream>

class Context;

class Generator : public Visitor
{
public:
    Generator(Context &c, std::ostream &os);

    virtual void visit(BlockNode &node) override;
    virtual void visit(NamespaceNode &node) override;
    virtual void visit(FuncNode &node) override;
    virtual void visit(TemplateFuncNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(ClassNode &node) override;
    virtual void visit(TemplateClassNode &node) override;

private:
    Context &c;
    std::ostream &os;
};

#endif // GENERATOR_H
