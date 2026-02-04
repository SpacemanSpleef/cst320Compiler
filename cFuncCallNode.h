#pragma once
class cFuncCallNode : public cExprNode {
protected:
    cSymbol* m_name;       // The function being called
    cAstNode* m_args;      // The list of arguments (params)

public:
    cFuncCallNode(cSymbol* name, cAstNode* args) : 
        m_name(name), m_args(args) {
        AddChild(m_name);
        if (m_args != nullptr) AddChild(m_args);
    }

    virtual string NodeType() override { return "func_call"; }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }
};