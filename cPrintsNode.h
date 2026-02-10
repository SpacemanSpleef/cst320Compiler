#pragma once
//**************************************
// cPrintNode.h
//
// Defines AST node for print statements
//
// Inherits from cStmtNode so print statments can go in statement lists, and be
// used anywhere a statement is legal.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cExprNode.h"
#include <string.h>
class cPrintsNode : public cStmtNode
{
    public:
        // param is the value to be printed
        cPrintsNode(string*& stringlit) : cStmtNode()
        {
           m_value = stringlit;
        }
        virtual string AttributesToString() 
        {
            return " value=\"" + *m_value + "\"";
        }
        virtual string NodeType() { return string("prints"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        string* m_value;        // value of integer constant (literal)

};
