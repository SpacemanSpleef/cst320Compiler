#pragma once
#include "cDeclNode.h"
#include "cParamsNode.h"
class cFuncDeclNode : public cDeclNode {
protected:
    cDeclNode* m_returnType;
    cSymbol* m_name;
    cParamsNode* m_params;
    cBlockNode* m_body;
    bool m_hasDefinition = false;

public:
    cFuncDeclNode(cDeclNode* type, cSymbol* name) : cDeclNode() {
    m_returnType = type;
    m_name = name;
    m_hasDefinition = false;

    this->AddChild(m_returnType->GetSymbol()); 
    this->AddChild(m_name);

    cSymbol* existing = g_symbolTable.FindLocal(name->GetName());
    if (existing != nullptr) {
        cDeclNode* oldDecl = existing->GetDecl();
        
        // 1. If it's not a function, that IS an error (e.g., int foo; int foo();)
        if (oldDecl != nullptr && !oldDecl->IsFunc()) {
            SemanticParseError("Symbol " + name->GetName() + " already defined in current scope");
        } 
        // 2. If it IS a function, just check the return type
        else if (oldDecl != nullptr) {
            cFuncDeclNode* oldFunc = dynamic_cast<cFuncDeclNode*>(oldDecl);
            if (oldFunc != nullptr && oldFunc->GetType() != type) {
                SemanticParseError(name->GetName() + " previously declared with different return type");
            }
            // DO NOT throw an error here. Multiple prototypes are legal.
        }
    } else {
        g_symbolTable.Insert(name);
        name->SetDecl(this);
    }
}

    // NEW: Helper to get the number of parameters
    int GetParamCount() {
        if (m_params == nullptr) return 0;
        return m_params->GetParamCount(); // Or whatever method your cParamsNode uses to count
    }

    void AddParams(cParamsNode* params) {
        if (params != nullptr) {
            for (int i = 0; i < params->NumChildren(); i++) {
                this->AddChild(params->GetChild(i)); // Now this is legal!
        }
    }
}
    
    void AddBody(cDeclsNode* decls, cStmtsNode* stmts) 
{ 
    m_hasDefinition = true;
    
    // Update the symbol to point to THIS node because this is the real implementation
    m_name->SetDecl(this);

    // Add children as discussed (Option A / Flattened)
    if (decls != nullptr) this->AddChild(decls);
    if (stmts != nullptr) this->AddChild(stmts);
}   

    virtual cSymbol* GetSymbol() override { return m_name; }
    virtual string NodeType() override { return "func"; }
    virtual void Visit(cVisitor* visitor) override {
    if (!m_hasDefinition) {
        cSymbol* sym = g_symbolTable.Find(m_name->GetName());
        if (sym != nullptr) {
            cFuncDeclNode* defNode = dynamic_cast<cFuncDeclNode*>(sym->GetDecl());
            if (defNode != nullptr && defNode != this && defNode->HasDefinition()) {
                visitor->Visit(defNode);
                return; 
            }
        }
    }    visitor->Visit(this);
}
    virtual bool IsFunc() override {return true;}
    virtual cDeclNode* GetDecl() override {return this;}
    bool HasDefinition() { return m_hasDefinition; }
    void SetHasDefinition(bool val) { m_hasDefinition = val; }
    cDeclNode* GetType() override { return m_returnType; }
    cSymbol* GetName(){return m_name;}
    void SetName(cSymbol* name) 
    { 
        m_name = name; 
    
    // Crucial: Update the child list so the XML prints the correct symbol
    // In your constructor, m_name was the 2nd child (index 1)
        if (NumChildren() > 1) {
            SetChild(1, m_name);
        }
    }
};