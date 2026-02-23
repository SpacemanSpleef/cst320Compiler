#pragma once
class cVarRefNode : public cExprNode {
protected:
    cSymbol* m_ref;
    cVarRefNode() : m_ref(nullptr) {}
public:
    cVarRefNode(cSymbol* ref) {
        m_ref = g_symbolTable.Find(ref->GetName());
        if (m_ref == nullptr) {
            SemanticParseError("Symbol " + ref->GetName() + " not defined");
            m_ref = ref; 
        }
        AddChild(m_ref);
    }

    void AddMember(cSymbol* sym) {
        AddChild(sym);
    }

    virtual string NodeType() override { return "varref"; }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }
    
    cSymbol* GetSymbol() { return m_ref; }
};