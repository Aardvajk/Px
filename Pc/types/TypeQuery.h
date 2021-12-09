#ifndef TYPEQUERY_H
#define TYPEQUERY_H

#include "visitors/Visitor.h"

class Context;
class Type;
class Node;

class TypeQuery : public Visitor
{
public:
    explicit TypeQuery(Context &c);

    Type *result() const { return r; }

    virtual void visit(IdNode &node) override;
    virtual void visit(TypeNode &node) override;
    virtual void visit(VarNode &node) override;

    static Type *query(Context &c, Node *node);
    static Type *assert(Context &c, Node *node);

private:
    Context &c;
    Type *r;
};

#endif // TYPEQUERY_H
