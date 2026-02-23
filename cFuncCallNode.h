#pragma once
class cFuncCallNode : public cExprNode {
protected:
    cSymbol* m_name;       // The function being called
    cAstNode* m_args;      // The list of arguments (params)

public:
    cFuncCallNode(cSymbol* name, cAstNode* args) : cExprNode() {
    m_name = name;
    m_args = args;
    AddChild(m_name);
    if (m_args != nullptr) {
        for (int i = 0; i < m_args->NumChildren(); i++) {
            AddChild(m_args->GetChild(i));
        }
    }
}
    virtual string NodeType() override { return "funcCall"; }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }
    virtual cDeclNode* GetType(){return m_name->GetDecl();};

};