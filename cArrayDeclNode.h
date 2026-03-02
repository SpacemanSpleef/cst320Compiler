#pragma once
#include "cDeclNode.h"
#include "cSymbol.h"

class cArrayDeclNode : public cDeclNode
{
public:
    cArrayDeclNode(cSymbol* typeId, int size, cSymbol* arrayId) : cDeclNode()
    {
        m_length = size;  // Changed from m_size to m_length
        m_name = arrayId;
        m_baseType = (typeId != nullptr) ? typeId->GetDecl() : nullptr;

        if (m_baseType == nullptr) {
            SemanticParseError("Symbol " + typeId->GetName() + " not defined");
        }

        if (g_symbolTable.FindLocal(arrayId->GetName())) {
            SemanticParseError("Symbol " + arrayId->GetName() + " already defined in current scope");
        } else {
            g_symbolTable.Insert(arrayId);
            arrayId->SetDecl(this);
        }

        // Add Children: Type Symbol and Array Name Symbol
        if (typeId) this->AddChild(typeId);
        this->AddChild(arrayId);
    }

    virtual string NodeType() override { return "array_decl"; }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }

    virtual string AttributesToString() override
    {
        std::string elementType = "unknown";
        if (m_baseType && m_baseType->GetSymbol())
            elementType = m_baseType->GetSymbol()->GetName();

        // Get size/offset from parent class, then add array-specific attributes
        string result = cDeclNode::AttributesToString();
        result += " name=\"" + m_name->GetName() + "\"" +
                  " length=\"" + std::to_string(m_length) + "\"" +
                  " element_type=\"" + elementType + "\"";
        return result;
    }

    virtual bool IsArray() override { return true; }
    virtual cDeclNode* GetDecl() override { return this; }
    virtual cDeclNode* GetType() override
    {
        return m_baseType ? m_baseType->GetDecl() : nullptr;
    }

    cSymbol* GetSymbol() { return m_name; }
    cDeclNode* GetBaseType() { return m_baseType; }
    int GetArraySize() { return m_length; }  // Renamed from GetSize
    virtual std::string GetName() {return m_name->GetName();}
    
protected:
    cDeclNode* m_baseType;
    int m_length;
    cSymbol* m_name;
};