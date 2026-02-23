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

    virtual string NodeType() override { return "index"; }
    virtual void Visit(cVisitor *visitor) override { visitor->Visit(this); }
};