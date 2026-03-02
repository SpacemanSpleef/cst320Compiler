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
    cDeclNode* m_type;   // e.g., "int", "char"
    cSymbol* m_name;   // e.g., "aaa", "bbb"

  public:
cVarDeclNode(cDeclNode* type, cSymbol* sym) : cDeclNode() 
{
    m_type = type;
    
    cSymbol* local = g_symbolTable.FindLocal(sym->GetName());
    if (local != nullptr) {
        SemanticParseError("Symbol " + sym->GetName() + " already defined in current scope");
        m_name = local; 
    } 
    else {
        m_name = sym;
        g_symbolTable.Insert(m_name);
    }

    m_name->SetDecl(this);
    this->AddChild(m_type->GetSymbol()); 
    this->AddChild(m_name);
}
    virtual std::string GetName() override {return m_name->GetName();}

    virtual string NodeType() override { return "var_decl"; }
    virtual string AttributesToString() override
    {
        string result = cDeclNode::AttributesToString();
        return result;
    }
    // Traverse children
    virtual void Visit(cVisitor *visitor) override {visitor->Visit(this);}    
    virtual cSymbol* GetSymbol() override { return m_name; }
    virtual bool IsVar() override {return true;}
    virtual cDeclNode* GetDecl() override {return this;}
    virtual cDeclNode* GetType() override {return m_type;}
};
