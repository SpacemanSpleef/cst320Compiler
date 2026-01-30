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

class cReturnNode : public cStmtNode
{
    public:
        // param is first stmt in the list of statements
        cReturnNode(cExprNode *expr) : cStmtNode()
        {
            AddChild(expr);
        }
        virtual string NodeType() { return string("return"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
