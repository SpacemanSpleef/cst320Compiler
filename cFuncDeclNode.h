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
        
        if (oldDecl != nullptr && !oldDecl->IsFunc()) {
            SemanticParseError("Symbol " + name->GetName() + " already defined in current scope");        
        } 
        else if (oldDecl != nullptr) {
            cFuncDeclNode* oldFunc = dynamic_cast<cFuncDeclNode*>(oldDecl);
            if (oldFunc != nullptr && oldFunc->GetType() != type) {
                SemanticParseError(name->GetName() + " previously declared with different return type");
            }
            
            this->SetName(existing); 
        }
    } else {
        g_symbolTable.Insert(name);
        name->SetDecl(this);
    }
}
    int GetParamCount() {
        if (m_params == nullptr) return 0;
        return m_params->GetParamCount(); 
    }

    void AddParams(cParamsNode* params) {
        if (params != nullptr) {
            for (int i = 0; i < params->NumChildren(); i++) {
                this->AddChild(params->GetChild(i));
        }
    }
}
    
   void AddBody(cDeclsNode* decls, cStmtsNode* stmts)
    {
        m_hasDefinition = true;
        m_name->SetDecl(this); 

        if (decls != nullptr) this->AddChild(decls);
        if (stmts != nullptr) this->AddChild(stmts);
    }

    virtual cSymbol* GetSymbol() override { return m_name; }
    virtual string NodeType() override { return "func"; }
    virtual void Visit(cVisitor* visitor) override {
    visitor->Visit(this);
}
    virtual bool IsFunc() override {return true;}
    virtual cDeclNode* GetDecl() override {return this;}
    bool HasDefinition() { return m_hasDefinition; }
    void SetHasDefinition(bool val) { m_hasDefinition = val; }
    cDeclNode* GetType() override { return m_returnType; }
    //cSymbol* GetName(){return m_name;}
    virtual std::string GetName() {return m_name->GetName();}
    void SetName(cSymbol* name) 
    { 
        m_name = name; 
    
        if (NumChildren() > 1) {
            SetChild(1, m_name);
        }
    }
};