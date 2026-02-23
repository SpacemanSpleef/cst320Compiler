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
class cStructDeclNode : public cDeclNode
{
  protected:
    cSymbol* m_name;   
    cDeclsNode* m_decls;


  public:
        cStructDeclNode(cSymbol* name, cDeclsNode* members) : cDeclNode()
    {
        m_name = name;
        m_decls = members;

        // Check for duplicate definition in the current scope
        if (g_symbolTable.FindLocal(name->GetName())) 
        {
            SemanticParseError("Symbol " + name->GetName() + " already defined in current scope");
        } 
        else 
        {
            // Insert the struct name into the symbol table so it can be used as a type
            g_symbolTable.Insert(m_name);
            m_name->SetDecl(this);
        }

        // Add the children for XML output
        // Most tests expect the struct name symbol to be a child as well
        this->AddChild(m_name);
        if (m_decls) this->AddChild(m_decls);
    }

    // Accessors
    //cSymbol* GetType() { return m_type; }
    //cSymbol* GetName() { return m_name; }

    // Return attributes for XML output
    virtual string AttributesToString() override
    {
        // No attributes of the var_decl node itself; type and name are children
        return "";
    }
    cSymbol* Lookup(string name)
{
    if (m_decls == nullptr) return nullptr;
    
    for (int i = 0; i < m_decls->NumChildren(); i++)
    {
        cDeclNode* decl = dynamic_cast<cDeclNode*>(m_decls->GetChild(i));
        if (decl && decl->GetSymbol()->GetName() == name)
            return decl->GetSymbol();
    }
    return nullptr;
}
    virtual string NodeType() override { return "struct_decl"; }

    // Traverse children
    virtual void Visit(cVisitor* visitor) override
    {
        visitor->Visit(this);    }
    virtual cSymbol* GetSymbol() override { return m_name; }
    virtual bool IsStruct() override {return true;}
    virtual cDeclNode* GetDecl() override {return this;}
    virtual cDeclNode* GetType() override {return this;}
    virtual bool IsType() override {return true;}
    cDeclsNode* GetMembers() {return m_decls;}

};
