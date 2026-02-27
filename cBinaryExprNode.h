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

#include "cSymbolTable.h"
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
        virtual cDeclNode* GetType() override
        {
            cOpNode* op = dynamic_cast<cOpNode*>(GetChild(1));
            if (op == nullptr) return nullptr;
            int opVal = op->GetOp();
        
            if (opVal == '>' || opVal == '<' || opVal == GE || opVal == LE ||
                opVal == EQUALS || opVal == NOT_EQUALS || opVal == AND || opVal == OR)
                return g_symbolTable.Find("int")->GetDecl();
            
            cDeclNode* leftType  = GetLeft()->GetType();
            cDeclNode* rightType = GetRight()->GetType();
            
            if (leftType != nullptr)  leftType  = leftType->GetType();
            if (rightType != nullptr) rightType = rightType->GetType();
            
            if (leftType == nullptr)  return rightType;
            if (rightType == nullptr) return leftType;
            
            if (leftType->IsFloat() && !rightType->IsFloat()) return leftType;
            if (rightType->IsFloat() && !leftType->IsFloat()) return rightType;
            
            return (leftType->GetSize() >= rightType->GetSize()) ? leftType : rightType;
        }
        cExprNode* GetLeft() {return dynamic_cast<cExprNode*>(GetChild(0));}
        cExprNode* GetRight() {return dynamic_cast<cExprNode*>(GetChild(2));}

};
