#pragma once
#include "cVarRefNode.h"
#include "cExprNode.h"

class cIndexNode : public cVarRefNode
{
    public:
        cIndexNode(cVarRefNode *base, cExprNode *index) : cVarRefNode(base, nullptr)
        {
            this->AddChild(base);
            this->AddChild(index);
        }

        virtual string NodeType() { return string("index"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};