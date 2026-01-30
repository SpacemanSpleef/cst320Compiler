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

class cIfStmtNode : public cStmtNode
{
    public:
        // param is first stmt in the list of statements
        cIfStmtNode(cExprNode *expr, cStmtsNode* stmts) : cStmtNode()
        {
            AddChild(expr);
            AddChild(stmts);
        }
        virtual string NodeType() { return string("if"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
