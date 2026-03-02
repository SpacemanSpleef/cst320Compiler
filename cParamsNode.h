#pragma once
class cParamsNode : public cAstNode {
protected:
    int m_size;
    int m_offset;
    bool m_isArgs;
public:
    cParamsNode(cAstNode* firstParam, bool isArgs = false) : cAstNode(), m_size(0), m_offset(0), m_isArgs(isArgs) {
        if (firstParam != nullptr) AddChild(firstParam);
    }
    
    void AddParam(cAstNode* param) {
        if (param != nullptr) AddChild(param);
    }

    virtual string NodeType() override { return m_isArgs ? "args" : "params"; }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }
    
    int GetParamCount()
    {
        return NumChildren();
    }
    
    int GetSize() { return m_size; }
    void SetSize(int size) { m_size = size; }
    int GetOffset() { return m_offset; }
    void SetOffset(int offset) { m_offset = offset; }
    
    virtual string AttributesToString() override 
    {
        if (m_size == 0) return "";
        return " size=\"" + std::to_string(m_size) + "\"";
    }
};