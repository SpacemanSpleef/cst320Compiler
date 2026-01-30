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

class cWhileNode : public cStmtNode
{
    public:
        // param is first stmt in the list of statements
        cWhileNode(cExprNode *expr, cStmtNode* stmt) : cStmtNode()
        {
            AddChild(expr);
            AddChild(stmt);
        }
        
        virtual string NodeType() { return string("while"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
