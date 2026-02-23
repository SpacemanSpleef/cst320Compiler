#pragma once
#include "cDeclNode.h"
#include "cSymbol.h"

class cArrayDeclNode : public cDeclNode
{
public:
    cArrayDeclNode(cSymbol* typeId, int size, cSymbol* arrayId)
    : cDeclNode()
    {
        m_size = size;
        m_name = arrayId;
        
        // 1. Validate the Base Type (Requirement #2)
        // typeId comes from the scanner as a TYPE_ID
        if (typeId == nullptr || typeId->GetDecl() == nullptr) {
            SemanticParseError("Symbol " + typeId->GetName() + " not defined");
            m_baseType = nullptr;
        } else {
            m_baseType = typeId->GetDecl();
        }
    
        // 2. Check for Duplicate Definition (Requirement #1)
        if (g_symbolTable.FindLocal(arrayId->GetName())) {
            SemanticParseError("Symbol " + arrayId->GetName() + " already defined in current scope");
        } else {
            // 3. Success: Insert and Link (Requirement #10)
            g_symbolTable.Insert(arrayId);
            arrayId->SetDecl(this);
        }
    
        // Add children for the XML/Visitor
        // Note: m_baseType is a cDeclNode, arrayId is a cSymbol
        if (m_baseType) this->AddChild(m_baseType);
        this->AddChild(arrayId);
    }

    virtual string NodeType() override { return "array_decl"; }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }

    virtual string AttributesToString() override
    {
        std::string elementType = "unknown";
        if (m_baseType && m_baseType->GetSymbol())
            elementType = m_baseType->GetSymbol()->GetName();

        return " name=\"" + m_name->GetName() + "\"" +
               " size=\"" + std::to_string(m_size) + "\"" +
               " element_type=\"" + elementType + "\"";
    }

    virtual bool IsArray() override { return true; }

    // A declaration returns itself
    virtual cDeclNode* GetDecl() override { return this; }

    // Type of the array is the type of its elements
    virtual cDeclNode* GetType() override
    {
        return m_baseType ? m_baseType->GetDecl() : nullptr;
    }

    cSymbol* GetSymbol() { return m_name; }
    cDeclNode* GetBaseType() { return m_baseType; }
    int GetSize() { return m_size; }

protected:
    cDeclNode* m_baseType; // Symbol for element type
    int m_size;          // Array length
    cSymbol* m_name;     // Symbol representing array
};