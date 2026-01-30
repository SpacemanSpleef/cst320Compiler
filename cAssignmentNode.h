#pragma once
//**************************************
// cStmtsNode.h
//
// Defines AST node that represents a list of statements
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cStmtNode.h"

class cAssignmentNode : public cStmtNode
{
    public:
        // param is first stmt in the list of statements
        cAssignmentNode(cAstNode* lval, cExprNode* expr) : cStmtNode()
        {
            AddChild(lval);
            AddChild(expr);
        }
        
        virtual string NodeType() { return string("assign"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
