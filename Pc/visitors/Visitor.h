#ifndef VISITOR_H
#define VISITOR_H

#include <utility>

class BlockNode;
class IdNode;
class NamespaceNode;
class FuncNode;
class TemplateFuncNode;
class ScopeNode;
class TypeNode;
class VarNode;
class ClassNode;
class TemplateClassNode;
class ExprNode;
class CallNode;
class IntLiteralNode;
class CharLiteralNode;
class ReturnNode;
class DerefNode;
class AddrNode;
class AssignNode;
class ThisNode;

class Visitor
{
public:
    virtual ~Visitor();

    virtual void visit(BlockNode &node);
    virtual void visit(IdNode &node);
    virtual void visit(NamespaceNode &node);
    virtual void visit(FuncNode &node);
    virtual void visit(TemplateFuncNode &node);
    virtual void visit(ScopeNode &node);
    virtual void visit(TypeNode &node);
    virtual void visit(VarNode &node);
    virtual void visit(ClassNode &node);
    virtual void visit(TemplateClassNode &node);
    virtual void visit(ExprNode &node);
    virtual void visit(CallNode &node);
    virtual void visit(CharLiteralNode &node);
    virtual void visit(IntLiteralNode &node);
    virtual void visit(ReturnNode &node);
    virtual void visit(DerefNode &node);
    virtual void visit(AddrNode &node);
    virtual void visit(AssignNode &node);
    virtual void visit(ThisNode &node);

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
