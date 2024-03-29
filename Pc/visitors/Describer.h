#ifndef DESCRIBER_H
#define DESCRIBER_H

#include "visitors/Visitor.h"

#include <string>
#include <sstream>

class Describer : public Visitor
{
public:
    Describer();

    std::string result() const { return r.str(); }

    virtual void visit(BlockNode &node) override;
    virtual void visit(IdNode &node) override;
    virtual void visit(NamespaceNode &node) override;
    virtual void visit(FuncNode &node) override;
    virtual void visit(TemplateFuncNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(TypeNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(ClassNode &node) override;
    virtual void visit(TemplateClassNode &node) override;
    virtual void visit(ExprNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(CharLiteralNode &node) override;
    virtual void visit(IntLiteralNode &node) override;
    virtual void visit(ReturnNode &node) override;
    virtual void visit(DerefNode &node) override;
    virtual void visit(AddrNode &node) override;
    virtual void visit(AssignNode &node) override;
    virtual void visit(ThisNode &node) override;

private:
    std::ostringstream r;
};

#endif // DESCRIBER_H
