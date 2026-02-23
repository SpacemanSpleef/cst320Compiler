#pragma once
#include "cVarRefNode.h"

class cIndexNode : public cVarRefNode
{
  public:
    cIndexNode(cVarRefNode *base, cExprNode *index) : cVarRefNode()
    {
        this->AddChild(base);
        this->AddChild(index);    
    }
    // Getter for the base (the array variable)
    cExprNode* GetBase() { 
        return dynamic_cast<cExprNode*>(GetChild(0)); 
    }

    // Getter for the index expression
    cExprNode* GetIndex() { 
        return dynamic_cast<cExprNode*>(GetChild(1)); 
    }

    // Helper to get the name for error messages
    string GetBaseName() {
        cVarRefNode* var = dynamic_cast<cVarRefNode*>(GetBase());
        if (var != nullptr && var->GetSymbol() != nullptr) {
            return var->GetSymbol()->GetName();
        }
        return "unknown";
    }
    virtual string NodeType() override { return "index"; }
    virtual void Visit(cVisitor *visitor) override { visitor->Visit(this); }
};