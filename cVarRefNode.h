#pragma once
class cVarRefNode : public cExprNode {
protected:
    cSymbol* m_ref;
    cVarRefNode() : m_ref(nullptr) {}
    int m_size = 0;
    int m_offset = 0;
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
    virtual cDeclNode* GetType() override {
    if (m_ref != nullptr) {
        return m_ref->GetDecl();
    }
    return nullptr;
}
    void SetSize(int size) { m_size = size; }
    void SetOffset(int offset) { m_offset = offset; }
    int GetSize() { return m_size; }
    int GetOffset() { return m_offset; }
    cSymbol* GetSymbol() { return m_ref; }
        virtual string AttributesToString() override 
    {
        if (m_size == 0 && m_offset == 0) return "";
        return " size=\"" + std::to_string(m_size) + 
               "\" offset=\"" + std::to_string(m_offset) + "\"";
    }

};