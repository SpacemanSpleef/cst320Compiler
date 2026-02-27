#pragma once
//*******************************************************
// Declaration of the visitor class.
// This class is used to implement the visitor pattern.
//
// As you add AST node types, you must add the forward declaration and
// the virtual Visit function.
// Lab 4 will work without this, BUT Lab 5 will go more smoothly if you
// keep this class current with all your AST node types.


// Forward (incomplete) declarations for all AST node types
class cAstNode;
class cBlockNode;
class cDeclNode;
class cDeclsNode;
class cExprNode;
class cIntExprNode;
class cOpNode;
class cPrintNode;
class cProgramNode;
class cStmtNode;
class cStmtsNode;
class cSymbol;
class cVarDeclNode;
class cAssignmentNode;
class cBinaryExprNode;
class cFuncCallNode;
class cFuncDeclNode;
class cVarRefNode;
class cIndexNode;
class cReturnNode;
class cIfStmtNode;
class cWhileNode;
class cFloatExprNode;
class cArrayDeclNode; 
class cStructDeclNode;
class cParamsNode;
class cPrintsNode;

class cVisitor
{
    public:
        cVisitor() {}

        virtual void VisitAllNodes(cAstNode *node);

        virtual void Visit(cAstNode *node);
        virtual void Visit(cBlockNode *node);
        virtual void Visit(cDeclNode *node);
        virtual void Visit(cDeclsNode *node);
        virtual void Visit(cExprNode *node);
        virtual void Visit(cIntExprNode *node);
        virtual void Visit(cOpNode *node);
        virtual void Visit(cPrintNode *node);
        virtual void Visit(cProgramNode *node);
        virtual void Visit(cStmtNode *node);
        virtual void Visit(cStmtsNode *node);
        virtual void Visit(cSymbol *node);
        virtual void Visit(cVarDeclNode* node);
        virtual void Visit(cAssignmentNode *node);
        virtual void Visit(cBinaryExprNode *node);
        virtual void Visit(cFuncCallNode *node);
        virtual void Visit(cFuncDeclNode *node);
        virtual void Visit(cVarRefNode *node);
        virtual void Visit(cIndexNode *node);
        virtual void Visit(cReturnNode *node);
        virtual void Visit(cIfStmtNode *node);
        virtual void Visit(cWhileNode *node);
        virtual void Visit(cFloatExprNode *node);
        virtual void Visit(cArrayDeclNode *node);
        virtual void Visit(cStructDeclNode *node);
        virtual void Visit(cParamsNode *node);
        virtual void Visit(cPrintsNode *node);
};