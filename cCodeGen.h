#pragma once
#include "cVisitor.h"
#include "astnodes.h"
#include "emit.h"

class cCodeGen : public cVisitor
{
public:
    cCodeGen(std::string filename) : cVisitor()
    {
        InitOutput(filename);
    }
    
    ~cCodeGen()
    {
        FinalizeOutput();
    }

    virtual void Visit(cProgramNode *node) override
    {
        EmitString(".function main\n");
        EmitString("main:\n");
        node->VisitAllChildren(this);
        EmitString("    PUSH 0\n");
        EmitString("    RETURNV\n");
    }
    
    virtual void Visit(cBlockNode *node) override
    {
        int blockSize = node->GetSize();
        if (blockSize > 0)
        {
            EmitString("    ADJSP ");
            EmitInt(blockSize);
            EmitString("\n");
        }
        node->VisitAllChildren(this);
    }
    
    virtual void Visit(cDeclsNode *node) override
    {
        node->VisitAllChildren(this);
    }
    
    virtual void Visit(cVarDeclNode *node) override
    {
    }
    
    virtual void Visit(cPrintNode *node) override
    {
        node->GetChild(0)->Visit(this);
        
        EmitString("    CALL @print\n");
        EmitString("    POP\n");
    }
    
    virtual void Visit(cAssignmentNode *node) override
    {
        cExprNode* lval = node->GetLVal();
        cExprNode* rval = node->GetExpr();
        
        rval->Visit(this);
        
        EmitString("    DUP\n");
        cVarRefNode* varRef = dynamic_cast<cVarRefNode*>(lval);
        if (varRef != nullptr)
        {
            int offset = varRef->GetOffset();
            EmitString("    PUSH ");
            EmitInt(offset);
            EmitString("\n");
            EmitString("    PUSHFP\n");
            EmitString("    PLUS\n");
            EmitString("    POPVARIND\n");
        }
        
        EmitString("    POP\n");
    }
    
    virtual void Visit(cIntExprNode *node) override
    {
        int value = node->GetValue();
        EmitString("    PUSH ");
        EmitInt(value);
        EmitString("\n");
    }
    
    virtual void Visit(cVarRefNode *node) override
    {
        int offset = node->GetOffset();
        EmitString("    PUSHVAR ");
        EmitInt(offset);
        EmitString("\n");
    }
    
    virtual void Visit(cBinaryExprNode *node) override
    {
        node->GetLeft()->Visit(this);
        node->GetRight()->Visit(this);
        
        cOpNode* op = dynamic_cast<cOpNode*>(node->GetChild(1));
        int opVal = op->GetOp();
        
        switch(opVal)
        {
            case '+':
                EmitString("    PLUS\n");
                break;
            case '-':
                EmitString("    MINUS\n");
                break;
            case '*':
                EmitString("    TIMES\n");
                break;
            case '/':
                EmitString("    DIVIDE\n");
                break;
            case '%':
                EmitString("    MOD\n");
                break;
            case '<':
                EmitString("    LT\n");
                break;
            case '>':
                EmitString("    GT\n");
                break;
            case EQUALS:
                EmitString("    EQ\n");
                break;
            case NOT_EQUALS:
                EmitString("    NE\n");
                break;
            case LE:
                EmitString("    LE\n");
                break;
            case GE:
                EmitString("    GE\n");
                break;
            case AND:
                EmitString("    AND\n");
                break;
            case OR:
                EmitString("    OR\n");
                break;
        }
    }
    
    virtual void Visit(cIfStmtNode *node) override
    {
        std::string elseLabel = GenerateLabel();
        std::string endLabel = GenerateLabel();
        
        cExprNode* cond = dynamic_cast<cExprNode*>(node->GetChild(0));
        cond->Visit(this);
        EmitString("    JUMPE @");
        EmitString(elseLabel);
        EmitString("\n");
        cStmtsNode* thenStmts = dynamic_cast<cStmtsNode*>(node->GetChild(1));
        thenStmts->Visit(this);
        if (node->NumChildren() > 2)
        {
            EmitString("    JUMP @");
            EmitString(endLabel);
            EmitString("\n");
            EmitString(elseLabel);
            EmitString(":\n");
            cStmtsNode* elseStmts = dynamic_cast<cStmtsNode*>(node->GetChild(2));
            elseStmts->Visit(this);
            EmitString(endLabel);
            EmitString(":\n");
        }
        else
        {
            EmitString(elseLabel);
            EmitString(":\n");
        }
    }
    
    virtual void Visit(cWhileNode *node) override
    {
        std::string startLabel = GenerateLabel();
        std::string endLabel = GenerateLabel();
        EmitString(startLabel);
        EmitString(":\n");
        cExprNode* cond = dynamic_cast<cExprNode*>(node->GetChild(0));
        cond->Visit(this);
        EmitString("    JUMPE @");
        EmitString(endLabel);
        EmitString("\n");
        cStmtNode* body = dynamic_cast<cStmtNode*>(node->GetChild(1));
        body->Visit(this);
        EmitString("    JUMP @");
        EmitString(startLabel);
        EmitString("\n");
        EmitString(endLabel);
        EmitString(":\n");
    }
    virtual void Visit(cPrintsNode *node) override
    {
        std::string str = node->GetString();
        std::string label = GenerateLabel();
        EmitString("    PUSH @");
        EmitString(label);
        EmitString("\n");
        EmitString("    OUTS\n");
        EmitString(".dataseg\n");
        EmitString(label);
        EmitString(":\n");
        EmitString(".string \"");
        EmitString(str);
        EmitString("\\n");
        EmitString("\"\n");
        EmitString(".codeseg\n");
    }
    // virtual void Visit(cFuncCallNode *node) override
    // {
    //     for (int i = node->GetNumArgs() - 1; i >= 0; i--)
    //     {
    //         node->GetArg(i)->Visit(this);
    //     }
    //     EmitString("    CALL @");
    //     EmitString(node->GetSymbol()->GetName());
    //     EmitString("\n");
    //     int numArgs = node->GetNumArgs();
    //     if (numArgs > 0)
    //     {
    //         EmitString("    ADJSP -");
    //         EmitInt(numArgs * 4); 
    //         EmitString("\n");
    //     }
    // }
    // virtual void Visit(cReturnNode *node) override
    // {
    //     cExprNode* expr = dynamic_cast<cExprNode*>(node->GetChild(0));
    //     expr->Visit(this);
    //     EmitString("    RETURNV\n");
    // }
    // virtual void Visit(cFuncDeclNode *node) override
    // {
    //     if (!node->HasDefinition()) return;
    //     EmitString(".function ");
    //     EmitString(node->GetName());
    //     EmitString("\n");
    //     EmitString(node->GetName());
    //     EmitString(":\n");
    //     int localSize = node->GetSize();
    //     if (localSize > 0)
    //     {
    //         EmitString("    ADJSP ");
    //         EmitInt(localSize);
    //         EmitString("\n");
    //     }
    //     int startChild = 2; 
    //     if (node->GetParams() != nullptr) startChild++; 

    //     for (int i = startChild; i < node->NumChildren(); i++)
    //     {
    //         node->GetChild(i)->Visit(this);
    //     }
    //     EmitString("    PUSH 0\n");
    //     EmitString("    RETURNV\n");
    // }
};