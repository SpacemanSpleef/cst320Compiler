#pragma once
class cVarRefNode : public cExprNode {
protected:
    cSymbol* m_ref;
    cVarRefNode() : m_ref(nullptr) {}
    int m_size = 0;
    int m_offset = 0;
    int m_rowSize;
public:
        cVarRefNode(cSymbol* ref) : m_size(0), m_offset(0), m_rowSize(0) {
        m_ref = g_symbolTable.Find(ref->GetName());
        if (m_ref == nullptr) {
            SemanticParseError("Symbol " + ref->GetName() + " not defined");
            m_ref = ref; 
        }
        AddChild(m_ref);
    }

    void SetRowSize(int size) { m_rowSize = size; }
    int GetRowSize() { return m_rowSize; }
    
    // Check if this is an array access
    bool IsArrayAccess() { return NumChildren() > 1; }
    
    virtual string AttributesToString() override 
    {
        string result = "";
        if (m_size != 0 || m_offset != 0) {
            result = " size=\"" + std::to_string(m_size) + 
                   "\" offset=\"" + std::to_string(m_offset) + "\"";
        }
        if (m_rowSize != 0) {
            result += " rowsizes=\"" + std::to_string(m_rowSize) + "\"";
        }
        return result;
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

};