#pragma once
class cFuncCallNode : public cExprNode {
protected:
    cSymbol* m_name;       // The function being called
    cAstNode* m_args;      // The list of arguments (params)

public:
    cFuncCallNode(cSymbol* name, cAstNode* args) : cExprNode() 
    {
        m_name = name;
        m_args = args;
        AddChild(m_name);
        if (m_args != nullptr) 
        {
            AddChild(m_args); 
        }
    }
    virtual string NodeType() override { return "funcCall"; }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }
    virtual cDeclNode* GetType() 
    {
    cFuncDeclNode* func = dynamic_cast<cFuncDeclNode*>(m_name->GetDecl());
    if (func != nullptr) return func->GetReturnType();
    return nullptr;
    }
    cSymbol* GetSymbol(){return m_name;}
    int GetNumArgs() 
    {
        if (m_args == nullptr) return 0;
        return m_args->NumChildren();
    }

    cExprNode* GetArg(int i) 
    {
        if (m_args == nullptr) return nullptr;
        return dynamic_cast<cExprNode*>(m_args->GetChild(i));
    }
};