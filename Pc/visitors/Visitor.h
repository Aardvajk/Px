#ifndef VISITOR_H
#define VISITOR_H

#include <utility>

class BlockNode;
class IdNode;
class GenericTagNode;
class NamespaceNode;
class FuncNode;
class TypeNode;
class VarNode;
class ClassNode;
class ScopeNode;
class ExprNode;
class CallNode;
class CharLiteralNode;
class IntLiteralNode;
class BoolLiteralNode;
class ReturnNode;
class IfNode;
class CommaNode;

class Visitor
{
public:
    virtual ~Visitor();

    virtual void visit(BlockNode &node);
    virtual void visit(IdNode &node);
    virtual void visit(GenericTagNode &node);
    virtual void visit(NamespaceNode &node);
    virtual void visit(FuncNode &node);
    virtual void visit(TypeNode &node);
    virtual void visit(VarNode &node);
    virtual void visit(ClassNode &node);
    virtual void visit(ScopeNode &node);
    virtual void visit(ExprNode &node);
    virtual void visit(CallNode &node);
    virtual void visit(CharLiteralNode &node);
    virtual void visit(IntLiteralNode &node);
    virtual void visit(BoolLiteralNode &node);
    virtual void visit(ReturnNode &node);
    virtual void visit(IfNode &node);
    virtual void visit(CommaNode &node);

    template<typename T, typename N, typename... Args> static void visit(N node, Args&&... args)
    {
        T v(std::forward<Args>(args)...);
        node->accept(v);
    }

    template<typename T, typename R, typename N, typename... Args> static R query(N node, Args&&... args)
    {
        T v(std::forward<Args>(args)...);
        node->accept(v);

        return v.result();
    }
};

#endif // VISITOR_H
