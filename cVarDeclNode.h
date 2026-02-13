#pragma once
#include "cAstNode.h"
#include "cSymbol.h"
#include <string>
using std::string;

//**************************************
// cVarDeclNode.h
//
// Represents a variable declaration: type + name
//
// Author: Theodore Gwynn
//**************************************
class cVarDeclNode : public cDeclNode
{
  protected:
    cSymbol* m_type;   // e.g., "int", "char"
    cSymbol* m_name;   // e.g., "aaa", "bbb"

  public:
    cVarDeclNode(cSymbol* type, cSymbol* name)
        : cDeclNode(), m_type(type), m_name(name) {
            AddChild(m_type);
            AddChild(m_name);
        }

    // Accessors
    cSymbol* GetType() { return m_type; }
    cSymbol* GetName() { return m_name; }

    // Return attributes for XML output
    virtual string AttributesToString() override
    {
        // No attributes of the var_decl node itself; type and name are children
        return "";
    }

    virtual string NodeType() override { return "var_decl"; }

    // Traverse children
    virtual void Visit(cVisitor* visitor) override
    {
        visitor->Visit(this);    }
    virtual cSymbol* GetSymbol() override { return m_name; }
    virtual bool IsVar() override {return true;}
    virtual cDeclNode* GetDecl(){return m_type->GetType();}
};
