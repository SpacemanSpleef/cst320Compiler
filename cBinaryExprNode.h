#pragma once
//**************************************
// cBinaryExprNode.h
//
// Defines an AST node for an integer constant (literals).
//
// Inherits from cExprNode so that integer constants can be used anywhere 
// expressions are used.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cOpNode.h"

class cBinaryExprNode : public cExprNode
{
    public:
        // param is the value of the integer constant
        cBinaryExprNode(cExprNode* lhs, int op, cExprNode* rhs) : cExprNode()
        {
            AddChild(lhs);
            AddChild(new cOpNode(op));
            AddChild(rhs);
        }
        virtual string NodeType() { return string("expr"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual cDeclNode* GetType() {return nullptr;}

};
